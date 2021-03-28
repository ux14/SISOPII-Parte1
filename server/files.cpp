#include <bits/stdc++.h>
#include <semaphore.h>
#include <errno.h>
#include <ctype.h>
#include "files.h"

#define USERNAME_MAX_SIZE 30

using namespace std;

Files::Files()
{
    sem_init(&rwUsers, 0, 1);
    sem_init(&rwFollowers, 0, 1);
    sem_init(&mutexFollowers, 0, 1);
}

void Files::createUser(string username)
{
    FILE *users_arq, *followers_arq;

    sem_wait(&rwUsers);
    users_arq = fopen("server/files/users.txt", "at");
    fprintf(users_arq, (username + '\n').c_str());
    fclose(users_arq);
    sem_post(&rwUsers);
}

void Files::addFollower(string follower, string followed)
{
    string followersArqDir = "server/files/followers/" + followed + ".txt";
    FILE *arq;

    sem_wait(&rwFollowers);
    arq = fopen(followersArqDir.c_str(), "at");
    fprintf(arq, (follower + '\n').c_str());
    fclose(arq);
    sem_post(&rwFollowers);
}

vector<string> Files::getUsers()
{
    vector<string> users;
    char user[USERNAME_MAX_SIZE];
    FILE *arq;

    sem_wait(&rwUsers);
    arq = fopen("server/files/users.txt", "rt");
    if (arq == NULL)
        return users;

    while (fgets(user, USERNAME_MAX_SIZE, arq) != NULL)
    {
        //remove \n
        *remove(user, user + strlen(user), '\n') = '\0';
        users.push_back(user);
    }
    sem_post(&rwUsers);
    fclose(arq);
    return users;
}

vector<string> Files::getFollowers(string username)
{
    vector<string> followers;
    char follower[USERNAME_MAX_SIZE];
    string followersArqDir = "server/files/followers/" + username + ".txt";
    FILE *arq;

    sem_wait(&mutexFollowers);
    followers_readers++;
    if (followers_readers == 1)
    {
        sem_wait(&rwFollowers);
    }
    sem_post(&mutexFollowers);

    arq = fopen(followersArqDir.c_str(), "rt");
    if (arq == NULL)
    {
        sem_wait(&mutexFollowers);
        followers_readers--;
        if (followers_readers == 0)
        {
            sem_post(&rwFollowers);
        }
        sem_post(&mutexFollowers);

        return followers;
    }

    while (fgets(follower, USERNAME_MAX_SIZE, arq) != NULL)
    {
        //remove \n
        *remove(follower, follower + strlen(follower), '\n') = '\0';
        followers.push_back(follower);
    }
    fclose(arq);

    sem_wait(&mutexFollowers);
    followers_readers--;
    if (followers_readers == 0)
    {
        sem_post(&rwFollowers);
    }
    sem_post(&mutexFollowers);

    return followers;
}
