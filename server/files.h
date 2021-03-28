#ifndef FILES_H
#define FILES_H

#include <string>
#include <vector>
#include <semaphore.h>

using namespace std;

class Files
{
private:
    sem_t rwUsers;
    sem_t rwFollowers;
    sem_t mutexFollowers;
    int followers_readers = 0;

public:
    Files();
    void createUser(string username);
    void addFollower(string follower, string followed);
    vector<string> getUsers();
    vector<string> getFollowers(string username);
};

#endif