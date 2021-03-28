#include "controller.h"

#define PAYLOAD_SIZE 128

Controller::Controller(string _username, SafeQueue<Message> *_sendMessage, SafeQueue<Message> *_printMessage, SafeQueue<string> *_receivedMessage, SafeQueue<string> *_userInput)
{
    username = _username;
    sendMessage = _sendMessage;
    printMessage = _printMessage;
    receivedMessage = _receivedMessage;
    userInput = _userInput;

    sendMessage->push(Message("LOGIN", username));

    thread input(&Controller::ParseUserInput, this);
    input.detach();

    thread server(&Controller::ParseServerMessage, this);
    server.detach();
}

void Controller::ParseUserInput()
{
    regex regex("^(SEND|FOLLOW) (.+)");
    smatch match;
    string input;

    sendMessage->push(Message("LOGIN",username, username));

    while (true)
    {
        input = userInput->pop();

        if (regex_search(input, match, regex) == true)
        {
            sendMessage->push(Message(match.str(1), match.str(2)));
        }
        else
        {
            cout << "ERROR: invalid command" << endl;
        }
    }
}

void Controller::ParseServerMessage()
{
    regex regex("(.+)##(.+)##(.+)");
    smatch match;

    while (true)
    {
        string server_message = receivedMessage->pop();

        if (regex_search(server_message, match, regex) == true)
        {
            Message message = Message(match.str(1), match.str(2).substr(0, PAYLOAD_SIZE), match.str(3));
            printMessage->push(message);
        }
    }
}
