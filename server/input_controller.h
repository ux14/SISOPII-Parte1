#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include <bits/stdc++.h>
#include "files.h"
#include "user_controller.h"
#include "notification_controller.h"

class InputController
{
private:
    UserController* user_controller;
    NotificationController* notification_controller;

public:
    void read_job(int fd, string user);
    string getCommand(string message);
    string getMessage(string message);
    InputController(UserController* u, NotificationController* v) : user_controller(u), notification_controller(v) {};
};

#endif
