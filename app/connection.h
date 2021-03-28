#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <string>
#include <cstring>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <thread>
#include <queue>
#include <unistd.h>
#include "message.h"
#include "safe_queue.h"

using namespace std;

class Connection
{
private:
    int socket_id;

    SafeQueue<Message> *sendMessage;
    SafeQueue<string> *receivedMessage;

    void Connect(string server_ip, int port);
    void ReceiveMessages();
    void SendMessage(string message);
    void SendMessages();

public:
    Connection(string server_ip, int port, SafeQueue<Message> *_sendMessage, SafeQueue<string> *_receivedMessage);
    ~Connection();
};

#endif
