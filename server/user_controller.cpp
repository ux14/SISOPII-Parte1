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
    std::lock_guard<std::mutex> sessions_lock(sessions_mutex);

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

bool UserController::nonLockUserExists(string username)
{
    return find(users.begin(), users.end(), username) != users.end();
}

bool UserController::userExists(string username)
{
    std::lock_guard<std::mutex> users_lock(users_mutex);
    return find(users.begin(), users.end(), username) != users.end();
}

bool UserController::login(string username)
{
    std::lock_guard<std::mutex> users_lock(users_mutex);

    if (nonLockUserExists(username))
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
    std::lock_guard<std::mutex> users_lock(users_mutex);

    if (nonLockUserExists(followed) && user != followed)
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
    std::lock_guard<std::mutex> sessions_lock(sessions_mutex);

    sessions.push_back(usuario);
}

vector<string> UserController::listFollowers(string username){
    return filesAccess.getFollowers(username);
}

vector<int> UserController::getSessions(string username)
{
    std::lock_guard<std::mutex> sessions_lock(sessions_mutex);

    vector<int> ans;

    for (auto i : sessions)
    {
        if ( i.user == username )
            ans.push_back(i.socketId);
    }

    return ans;
}