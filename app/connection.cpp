#include "connection.h"

#define BUFFER_SIZE 256

Connection::Connection(string server_ip, int port, SafeQueue<Message> *_sendMessage, SafeQueue<string> *_receivedMessage)
{
    Connect(server_ip, port);

    sendMessage = _sendMessage;
    receivedMessage = _receivedMessage;

    thread recv(&Connection::ReceiveMessages, this);
    recv.detach();

    thread snd(&Connection::SendMessages, this);
    snd.detach();
}

Connection::~Connection()
{
    SendMessage("LOGOUT");
    close(socket_id);
    exit(0);
}

void Connection::Connect(string server_ip, int port)
{
    struct hostent *server = gethostbyname(server_ip.c_str());
    struct sockaddr_in serv_addr;

    if (server == NULL)
    {
        cout << "ERROR: no such host" << endl;
        exit(-1);
    }

    if ((socket_id = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        cout << "ERROR: failed to open socket" << endl;
        exit(-1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);
    bzero(&(serv_addr.sin_zero), 8);

    if (connect(socket_id, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cout << "ERROR: failed to connect" << endl;
        close(socket_id);
        exit(-1);
    }
}

void Connection::ReceiveMessages()
{
    char buffer[BUFFER_SIZE];

    while (true)
    {
        int n = read(socket_id, buffer, BUFFER_SIZE);

        if (n <= 0)
        {
            cout << "ERROR: failed to read from socket" << endl;
            close(socket_id);
            exit(-1);
        }

        receivedMessage->push(string(buffer));
        bzero(buffer, BUFFER_SIZE);
    }
}

void Connection::SendMessage(string message)
{
    int n = write(socket_id, message.c_str(), message.length());

    if (n <= 0)
    {
        cout << "ERROR: failed to write to socket" << endl;
        close(socket_id);
        exit(-1);
    }
}

void Connection::SendMessages()
{
    while (true)
    {
        Message message = sendMessage->pop();
        string buffer = message.type + "##" + message.payload;
        SendMessage(buffer);
    }
}
