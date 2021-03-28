#ifndef LOGIN_H
#define LOGIN_H

#include <bits/stdc++.h>

using namespace std;

class Login
{
private:
    vector<string> users;
    vector<struct socketUser> *sessions;
    int isLoggedIn(string username);

public:
    Login(vector<struct socketUser> *_sessions);
    bool login(string username);
    bool userExists(string username);
};

#endif
