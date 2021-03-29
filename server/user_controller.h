#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include <bits/stdc++.h>
#include <mutex>
#include "files.h"

using namespace std;

class UserController
{
private:
    Files filesAccess;
    
    std::mutex users_mutex;
    vector<string> users;

    std::mutex sessions_mutex;
    vector<struct socketUser> sessions;
    
    int isLoggedIn(string username);

    bool nonLockUserExists(string username);

public:
    UserController();
    bool login(string username);
    bool userExists(string username);
    bool follow(string user, string followed);
    void registerSession(socketUser usuario);
    vector<string> listFollowers(string username);
    vector<int> getSessions(string username);
};

#endif
