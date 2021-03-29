#include <bits/stdc++.h>
#include <semaphore.h>
#include <errno.h>
#include <ctype.h>
#include "files.h"

#define USERNAME_MAX_SIZE 30

#include <mutex>

using namespace std;

void Files::createUser(string username)
{
    FILE *users_arq, *followers_arq;

    std::lock_guard<std::mutex> user_lock(rwUsers);

    users_arq = fopen("server/files/users.txt", "a+");
    fprintf(users_arq, "%s\n", username.c_str());
    fclose(users_arq);

    std::lock_guard<std::mutex> follower_lock(rwFollowers);
    followers_arq = fopen( ("server/files/followers/" +  username + ".txt").c_str(), "w+");
    fclose(followers_arq);

}

void Files::addFollower(string follower, string followed)
{
    string followersArqDir = "server/files/followers/" + followed + ".txt";
    FILE *arq;

    auto followers = getFollowers(followed);
    if( find(followers.begin(),followers.end(),follower) == followers.end() )
    {
        std::lock_guard<std::mutex> data_lock(rwFollowers);

        arq = fopen(followersArqDir.c_str(), "a+");
        fprintf(arq, "%s\n", follower.c_str());
        fclose(arq);
    }
}

vector<string> Files::getUsers()
{
    vector<string> users;
    char user[USERNAME_MAX_SIZE];
    FILE *arq;

    std::lock_guard<std::mutex> data_lock(rwUsers);

    arq = fopen("server/files/users.txt", "r+");
    if (arq == NULL)
        return users;

    while (fgets(user, USERNAME_MAX_SIZE, arq) != NULL)
    {
        //remove \n
        *remove(user, user + strlen(user), '\n') = '\0';
        users.push_back(user);
    }

    fclose(arq);
    return users;
}

vector<string> Files::getFollowers(string username)
{
    vector<string> followers;
    char follower[USERNAME_MAX_SIZE];
    string followersArqDir = "server/files/followers/" + username + ".txt";
    FILE *arq;

    std::lock_guard<std::mutex> data_lock(mutexFollowers);

    arq = fopen(followersArqDir.c_str(), "r+");

    while (fgets(follower, USERNAME_MAX_SIZE, arq) != NULL)
    {
        //remove \n
        *remove(follower, follower + strlen(follower), '\n') = '\0';
        followers.push_back(follower);
    }
    fclose(arq);

    return followers;
}
