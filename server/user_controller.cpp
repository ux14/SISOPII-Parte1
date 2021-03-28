#include <bits/stdc++.h>
#include "files.h"
#include "user_controller.h"
#include "socketUser.h"

UserController::UserController()
{
    users = filesAccess.getUsers();
}

int UserController::isLoggedIn(string username)
{
    int cont = 0;
    for (int i = 0; i < (int)sessions.size(); i++)
    {
        if (sessions[i].user == username)
        {
            cont++;
        }
    }
    return cont;
}

bool UserController::userExists(string username)
{
    return find(users.begin(), users.end(), username) != users.end();
}

bool UserController::login(string username)
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
        filesAccess.createUser(username);
        users.push_back(username);
        return true;
    }
}

bool UserController::follow(string user, string followed)
{
    if (userExists(followed) && user != followed)
    {
       filesAccess.addFollower(user, followed);
        return true;
    }
    else
    {
        return false;
    }
}

void UserController::registerSession(socketUser usuario)
{
    sessions.push_back(usuario);
}

vector<string> UserController::listFollowers(string username){
    return filesAccess.getFollowers(username);
}