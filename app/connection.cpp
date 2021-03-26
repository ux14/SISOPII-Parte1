#include "connection.h"
#include <cstring>
#define BUFFER_SIZE 256

Connection::Connection(string server_ip, int port, SafeQueue<Message> *_sendMessage, SafeQueue<string> *_receivedMessage)
{
    Connect(server_ip, port);

    sendMessage = _sendMessage;
    receivedMessage = _receivedMessage;

    thread listen(&Connection::ReceiveMessages, this);
    listen.detach();

    SendMessages();
}

Connection::~Connection()
{
    close(socket_id);
}

void Connection::Connect(string server_ip, int port)
{
    struct sockaddr_in serv_addr;
    struct hostent *server;

    server = gethostbyname(server_ip.c_str());
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    if ((socket_id = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("ERROR opening socket\n");
        return;
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(port);

    bzero(&(serv_addr.sin_zero), 8);

    if (connect(socket_id, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("ERROR connecting\n");
        return;
    }
}

void Connection::ReceiveMessages()
{
    int n;
    char buffer[BUFFER_SIZE];

    while (true)
    {
        n = read(socket_id, buffer, BUFFER_SIZE);

        if (n <= 0)
        {
            cout << "ERROR: failed to read from socket." << endl;
            close(socket_id);
            exit(-1);
        }

        receivedMessage->push(string(buffer));
        bzero(buffer, BUFFER_SIZE);
    }
}

void Connection::SendMessages()
{
    while (true)
    {
        Message message = sendMessage->pop();
        string buffer = message.type + "|" + message.payload;

        int n = write(socket_id, buffer.c_str(), buffer.length());

        if (n <= 0)
        {
            cout << "ERROR: failed to write to socket." << endl;
            close(socket_id);
            exit(-1);
        }
    }
}
