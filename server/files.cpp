#include <bits/stdc++.h>

using namespace std;

void createUser(string username)
{
    FILE *users_arq, *followers_arq;

    //Add user to users file
    users_arq = fopen("server/files/users.txt", "at");
    fprintf(users_arq, (username + '\n').c_str());
    fclose(users_arq);

    // //Creates follower file
    string followersArqDir = "server/files/followers/" + username + ".txt";
    followers_arq = fopen(followersArqDir.c_str(), "wt");
    fclose(followers_arq);
}

void follow(string follower, string followed)
{
    string followersArqDir = "server/files/followers/" + followed + ".txt";

    FILE *arq;
    arq = fopen(followersArqDir.c_str(), "at");
    fprintf(arq, (follower + '\n').c_str());
    fclose(arq);
}

vector<string> getUsers()
{
    vector<string> users;
    char user[30];
    FILE *arq;
    arq = fopen("server/files/users.txt", "rt");
    while (fgets(user, 30, arq) != NULL)
    {
        //remove \n
        *remove(user, user + strlen(user), '\n') = '\0';
        users.push_back(user);
    }
    fclose(arq);
    return users;
}

vector<string> getFollowers(string username)
{
    vector<string> followers;
    char follower[30];
    string followersArqDir = "server/files/followers/" + username + ".txt";

    FILE *arq;
    arq = fopen(followersArqDir.c_str(), "rt");
    while (fgets(follower, 30, arq) != NULL)
    {
        //remove \n
        *remove(follower, follower + strlen(follower), '\n') = '\0';
        followers.push_back(follower);
    }
    fclose(arq);
    return followers;
}
