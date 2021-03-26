#include "controller.h"

using namespace std;

Controller::Controller(string _username, SafeQueue<Message> *_sendMessage, SafeQueue<Message> *_printMessage, SafeQueue<string> *_receivedMessage, SafeQueue<string> *_userInput)
{
    username = _username;
    sendMessage = _sendMessage;
    printMessage = _printMessage;
    receivedMessage = _receivedMessage;
    userInput = _userInput;

    thread input(&Controller::ParseUserInput, this);
    input.detach();

    thread server(&Controller::ParseServerMessage, this);
    server.detach();
}

void Controller::ParseUserInput()
{
    regex regex("(SEND|FOLLOW) (.+)");
    smatch match;
    string input;

    while (true)
    {
        input = userInput->pop();

        if (regex_search(input, match, regex) == true)
        {
            sendMessage->push(Message(match.str(1), match.str(2), username));
        }
        else
        {
            cout << "ERROR: Invalid command." << endl;
        }
    }
}

void Controller::ParseServerMessage()
{
    regex regex("(SEND)##(.+)##(.+)");
    smatch match;

    while (true)
    {
        string server_message = receivedMessage->pop();

        if (regex_search(server_message, match, regex) == true)
        {
            Message message = Message(match.str(1), match.str(2), match.str(3));
            printMessage->push(message);
        }
        else
        {
            cout << "ERROR: Invalid received message" << endl;
        }
    }
}
