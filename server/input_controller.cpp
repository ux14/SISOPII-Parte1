#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include "socketUser.h"
#include "input_controller.h"
#include "user_controller.h"
#include "notification_controller.h"

using namespace std;

string InputController::getCommand(string message)
{
    int position = message.find("##");
    return message.substr(0, position);
}

string InputController::getMessage(string message)
{
    int position = message.find("##");
    if (getCommand(message) == "FOLLOW")
    {
        return message.substr(position + 2 + 1);
    }
    else if (getCommand(message) == "LOGOUT")
    {
        return "";
    }
    else
    {
        return message.substr(position + 2);
    }
}

void InputController::read_job(int sockfd, string user)
{
    char buffer[256];

    int n;

    while (1)
    {
        bzero(buffer, 256);
        /* read from the socket */
        n = read(sockfd, buffer, 256);
        if (n <= 0)
        {
            if (n < 0)
                printf("ERROR reading from socket\n");
            close(sockfd);
            break;
        }

        string command = getCommand(string(buffer));
        string message = "";
        message = getMessage(string(buffer));

        cout << "comando: " << command << ' ' << "mensagem: " << message << '\n';
        if (command == "SEND")
        {
            notification n;
            bzero(n.msg,256);
            std::copy(message.begin(), message.end(), n.msg);
            n.length = message.length();
            //Acontence algo com as mensagens
            notification_controller->producerQueue.push(std::make_pair(user, n));
        }
        else if (command == "FOLLOW")
        {
            string error = user_controller->follow(user, message);
            if (error.size() > 0)
            {
                error = "ERROR##" + error + "##" + user;
                n = write(sockfd, error.c_str(), error.size());
            }
        }
        else if (command == "LOGOUT")
        {
            socketUser s;
            s.socketId = sockfd;
            s.user = user;

            user_controller->unregisterSession(s);
            close(sockfd);
            break;
        }
        printf("Here is the message: %s\n", buffer);
        bzero(buffer, 256);
    }
}