#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <map>
#include <set>
#include <list>
#include <string>
#include <memory>

#include "../include/data.h"

// TODO: (?) mudar safe_queue para pasta include
#include "../app/safe_queue.h"
#include "notification_controller.h"

using user_t = std::string;
using notification_id = uint32_t;

void NotificationController::lockReceivedList(user_t user)
{
    std::lock_guard<std::mutex> data_lock( data_mutex );
    data[user].received_mutex.lock();
}

void NotificationController::unlockReceivedList(user_t user)
{
    std::lock_guard<std::mutex> data_lock( data_mutex );
    data[user].received_mutex.unlock();
} 

std::pair<user_t,notification> NotificationController::consume(user_t user)
{
    std::pair<user_t,notification> n;

    std::pair<user_t,notification_id> p;
    std::shared_ptr<SafeQueue<std::pair<user_t,notification_id>>> q;
    
    {
        std::lock_guard<std::mutex> data_lock( data_mutex );
        q = data[user].consumer_queue;
    }

    p = q->pop();

    user_t u = p.first;
    notification_id nid = p.second;

    this->lockReceivedList(u);

    {
        std::lock_guard<std::mutex> data_lock( data_mutex );

        auto it = data[u].received_notifications->begin();
        if( (it = data[u].received_notifications->find(nid)) != data[u].received_notifications->end() )
        {
            n.first = u;
            n.second = (*it).second;

            if( --(it->second.pending) == 0)
            {
                data[u].received_notifications->erase(it);
            }
        }
    }

    this->unlockReceivedList(u);

    return n;
}

void NotificationController::produce(user_t user, notification noti)
{
        static uint32_t id = 0;

        // achar todos os seguidores de 'user' aqui
        auto followers = this->user_controller->listFollowers(user); //= ?

        // inserir na fila de pendentes de 'user'

        noti.id = id++;
        noti.pending = followers.size();
        noti.timestamp = (uint32_t) time(nullptr);

        this->lockReceivedList(user);
        data[user].received_notifications->insert( std::make_pair(noti.id,noti));
        this->unlockReceivedList(user);

        for(auto it = followers.begin(); it != followers.end(); it++)
        {            
            user_t follower = *it;

            std::lock_guard<std::mutex> data_lock( data_mutex );
            // TODO
            // colocar na fila de mensagens pendentes ao envio do n-ésimo follower

            data[follower].consumer_queue->push( std::make_pair(user,noti.id) );
        }
}

void NotificationController::consumerThread(user_t user)
{
    std::pair<user_t,notification> noti;

    while(1)
    {
        std::pair<user_t,notification> pair_user_notification = this->consume(user);

        auto src_user = pair_user_notification.first;
        auto noti = pair_user_notification.second;

        string s = string("SEND##") + string(noti.msg) + string("##") + src_user;

        // achar sockets/sessões do usuário u e mandar a notificação
        for(auto fd : this->user_controller->getSessions(user))
        {
            int n = write(fd, s.c_str(), s.size());
            if(n < 0)
            {
                printf("ERROR in write to socket %d user %s", fd, user.c_str());
            }
        }
    }
}

void NotificationController::producerThread()
{
    auto p = this->producerQueue.pop();
    auto user = p.first;
    auto noti = p.second;

    while(1)
    {
        this->produce(user,noti);

        p = this->producerQueue.pop();
        user = p.first;
        noti = p.second;
    }
}