#ifndef SOCKETUSER_H
#define SOCKETUSER_H

#include <bits/stdc++.h>
#include <pthread.h>

struct socketUser
{
    int socketId;
    std::string user;
    pthread_t thread;
};

#endif
