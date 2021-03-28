#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include <bits/stdc++.h>
#include "files.h"

using namespace std;

class UserController
{
private:
    Files filesAccess;
    vector<string> users;
    vector<struct socketUser> *sessions;
    int isLoggedIn(string username);

public:
    UserController(vector<struct socketUser> *_sessions);
    bool login(string username);
    bool userExists(string username);
    bool follow(string user, string followed);
    vector<string> listFollowers(string username);
};

#endif
