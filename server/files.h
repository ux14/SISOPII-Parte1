#ifndef FILES_H
#define FILES_H

#include <string>
#include <vector>
#include <mutex>

using namespace std;

class Files
{
private:
    std::mutex rwUsers;
    std::mutex rwFollowers;
    std::mutex mutexFollowers;
    int followers_readers = 0;

public:
    void createUser(string username);
    void addFollower(string follower, string followed);
    vector<string> getUsers();
    vector<string> getFollowers(string username);
};

#endif