#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
#include <string>
#include <thread>
#include "message.h"
#include "safe_queue.h"

using namespace std;

class Interface
{
private:
    SafeQueue<Message> *printMessage;
    SafeQueue<string> *userInput;

    void GetInput();
    void PrintMessage();

public:
    Interface(SafeQueue<string> *_userInput, SafeQueue<Message> *_printMessage);
};

#endif
