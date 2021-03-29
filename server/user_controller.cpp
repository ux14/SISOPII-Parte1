#include <bits/stdc++.h>
#include "files.h"
#include "user_controller.h"
#include "socketUser.h"

UserController::UserController()
{
    users = filesAccess.getUsers();
}

vector<string> UserController::getUsers()
{
    std::lock_guard<std::mutex> users_lock(users_mutex);
    return users;
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

string UserController::follow(string user, string followed)
{
    std::lock_guard<std::mutex> users_lock(users_mutex);

    if (!nonLockUserExists(followed))
    {
        return followed + " you are trying to follow dosen't exist";
    }
    else if (user == followed)
    {
        return "You can't follow yourself";
    }
    else if (isFollowing(user, followed))
    {
        return "You already follow " + followed;
    }

    filesAccess.addFollower(user, followed);
    return "";
}

void UserController::registerSession(socketUser usuario)
{
    std::lock_guard<std::mutex> sessions_lock(sessions_mutex);

    sessions.push_back(usuario);
}

void UserController::unregisterSession(socketUser usuario)
{
    std::lock_guard<std::mutex> sessions_lock(sessions_mutex);
    for (auto it = sessions.begin(); it != sessions.end(); it++)
    {
        if ((*it).socketId == usuario.socketId && (*it).user == usuario.user)
        {
            sessions.erase(it);
            break;
        }
    }
}

vector<string> UserController::listFollowers(string username)
{
    return filesAccess.getFollowers(username);
}

bool UserController::isFollowing(string username, string followed)
{
    auto followers = filesAccess.getFollowers(followed);
    return find(followers.begin(), followers.end(), username) != followers.end();
}

vector<int> UserController::getSessions(string username)
{
    std::lock_guard<std::mutex> sessions_lock(sessions_mutex);

    vector<int> ans;

    for (auto i : sessions)
    {
        if (i.user == username)
            ans.push_back(i.socketId);
    }

    return ans;
}