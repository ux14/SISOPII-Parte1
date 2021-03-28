#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

using namespace std;

class Message
{
private:
public:
    string type;
    string payload;
    string username;

    Message(string type, string payload, string username)
    {
        this->type = type;
        this->payload = payload;
        this->username = username;
    }

    Message(string type, string payload)
    {
        this->type = type;
        this->payload = payload;
    }
};

#endif
