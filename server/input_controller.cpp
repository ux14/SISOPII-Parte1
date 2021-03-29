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
    return message.substr(position + 2);
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
        string message = getMessage(string(buffer));

        cout << "comando: " << command << ' ' << "mensagem: " << message << '\n';
        if (command == "SEND")
        {
            notification n;
            std::copy(message.begin(), message.end(), n.msg);
            n.length = message.length();
            //Acontence algo com as mensagens
            notification_controller->producerQueue.push(std::make_pair(user, n));
        }
        else if (command == "FOLLOW")
        {
            user_controller->follow(user, message);
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