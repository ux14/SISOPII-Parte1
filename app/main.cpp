#include <iostream>
#include <string>

#include "message.h"
#include "validation.h"
#include "safe_queue.h"

#include "connection.h"
#include "controller.h"
#include "interface.h"

using namespace std;

void validateInputs(string username, string server_ip, string port);

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cout << "./app <username> <server_ip> <port>" << endl;
        return -1;
    }

    string username = argv[1];
    string server_ip = argv[2];
    string port = argv[3];

    validateInputs(username, server_ip, port);

    SafeQueue<Message> sendMessage, printMessage;
    SafeQueue<string> receivedMessage, userInput;
    Interface(&userInput, &printMessage);

    Controller(username, &sendMessage, &printMessage, &receivedMessage, &userInput);
    Connection(server_ip, stoi(port), &sendMessage, &receivedMessage);

    return 0;
}