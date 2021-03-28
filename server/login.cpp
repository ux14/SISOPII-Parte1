#include <bits/stdc++.h>
#include "files.h"
#include "login.h"
#include "socketUser.h"

Login::Login(vector<struct socketUser> * _sessions)
{
    sessions = _sessions;
    users = getUsers();
}

int Login::isLoggedIn(string username)
{
    int cont = 0;
    for (int i = 0; i < (*sessions).size(); i++)
    {
        if ( (*sessions)[i].user == username)
        {
            cont++;
        }
    }
    return cont;
}

bool Login::userExists(string username)
{
    return find(users.begin(), users.end(), username) != users.end();
}

bool Login::login(string username)
{

    if (userExists(username))
    {
        if (isLoggedIn(username) >= 2)
        {
            //Usuário com duas sessões abertas
            return false;
        }
        else
        {
            //Permissão para logar concedida
            return true;
        }
    }
    else
    {
        createUser(username);
        users.push_back(username);
        return true;
    }
}

