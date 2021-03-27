#include<bits/stdc++.h>

using namespace std;

void createUsuer(string username){
    FILE *users_arq, *followers_arq;

    //Add user to users file
    users_arq = fopen("files/users.txt", "at");
    fprintf(users_arq, (username + '\n').c_str());
    fclose(users_arq);

    //Creates follower file
    string followersArqDir = "files/followers/" + username + ".txt";
    followers_arq = fopen(followersArqDir.c_str(), "wt");
    fclose(followers_arq);
}

void follow(string follower, string followed)
{
    string followersArqDir = "files/followers/" + followed + ".txt";

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
    arq = fopen("files/users.txt", "rt");
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
    string followersArqDir = "files/followers/" + username + ".txt";

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

int main()
{

    createUsuer("User1");
    createUsuer("User2");
    createUsuer("User3");

    follow("User2", "User1");
    follow("User3", "User1");
    follow("User3", "User2");

    vector<string> users = getUsers();
    vector<string> followers = getFollowers("User1");

    cout << "All users:" << endl;
    for (int i = 0; i < users.size(); i++)
    {
        cout << users[i] << endl;
    }

    cout << "User 1 followers:" << endl;
    for (int i = 0; i < followers.size(); i++)
    {
        cout << followers[i] << endl;
    }

    return 0;
}
