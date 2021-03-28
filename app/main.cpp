#include <iostream>
#include <string>
#include <signal.h>

#include "message.h"
#include "validation.h"
#include "safe_queue.h"

#include "connection.h"
#include "controller.h"
#include "interface.h"

using namespace std;

int exit_signal;
mutex exit_mutex;
condition_variable exit_cv;

void signalHandler(int sig)
{
    unique_lock<mutex> lock(exit_mutex);
    exit_signal = 1;
    exit_cv.notify_one();
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cout << "./app <username> <server_ip> <port>" << endl;
        return -1;
    }

    signal(SIGINT, signalHandler);

    string username = argv[1];
    string server_ip = argv[2];
    string port = argv[3];

    validateInputs(username, server_ip, port);

    SafeQueue<Message> sendMessage, printMessage;
    SafeQueue<string> receivedMessage, userInput;

    Interface(&userInput, &printMessage);
    Controller(username, &sendMessage, &printMessage, &receivedMessage, &userInput);
    Connection *connection = new Connection(server_ip, stoi(port), &sendMessage, &receivedMessage);

    unique_lock<mutex> lock(exit_mutex);
    exit_cv.wait(lock, [&] { return exit_signal == 1; });

    delete connection;

    return 0;
}