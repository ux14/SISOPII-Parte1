#include "interface.h"

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
    char inp[160];

    while (true)
    {
        cout << "> ";
        fgets(inp, 155, stdin);
        userInput->push(inp);
    }
}

void Interface::PrintMessage()
{
    while (true)
    {
        Message message = printMessage->pop();
        cout << "[" + message.username + "] " + message.payload << endl;
    }
}
