#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>
#include <iostream>
#include <regex>
#include <thread>
#include "message.h"
#include "safe_queue.h"

class Connection;

using namespace std;

class Controller
{
private:
    string username;

    SafeQueue<Message> *sendMessage, *printMessage;
    SafeQueue<string> *receivedMessage, *userInput;

    void ParseUserInput();
    void ParseServerMessage();

public:
    Controller(string _username, SafeQueue<Message> *_sendMessage, SafeQueue<Message> *_printMessage, SafeQueue<string> *_receivedMessage, SafeQueue<string> *_userInput);
};

#endif
