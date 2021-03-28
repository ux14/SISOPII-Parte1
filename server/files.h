#ifndef FILES_H
#define FILES_H

#include <string>
#include <vector>

using namespace std;

void createUser(string username);
void addFollower(string follower, string followed);
vector<string> getUsers();
vector<string> getFollowers(string username);

#endif