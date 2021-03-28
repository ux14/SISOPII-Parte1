#include "interface.h"

#define BUFFER_SIZE 256

Interface::Interface(SafeQueue<string> *_userInput, SafeQueue<Message> *_printMessage)
{
    printMessage = _printMessage;
    userInput = _userInput;

    thread input(&Interface::GetInput, this);
    input.detach();

    thread print(&Interface::PrintMessage, this);
    print.detach();
}

void Interface::GetInput()
{
    char inp[BUFFER_SIZE];

    while (true)
    {
        fgets(inp, BUFFER_SIZE - 1, stdin);

        if (feof(stdin))
        {
            raise(SIGINT);
        }
        else
        {
            userInput->push(inp);
        }
    }
}

void Interface::PrintMessage()
{
    while (true)
    {
        Message message = printMessage->pop();

        if (message.type == "SEND")
        {
            cout << "[" + message.username + "] " + message.payload << endl;
        };

        if (message.type == "ERROR")
        {
            cout << "SERVER ERROR: " + message.payload << endl;
        };
    }
}
