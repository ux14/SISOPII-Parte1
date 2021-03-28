#ifndef NOTIFICATION_CONTROLLER_H
#define NOTIFICATION_CONTROLLER_H

#include <ctime>
#include <cstdint>
#include <sys/types.h>

#include <map>
#include <set>
#include <queue>
#include <string>
#include <memory>
#include <mutex>

#include "../include/data.h"

// TODO: mudar safe_queue para pasta include
#include "../app/safe_queue.h"
#include "user_controller.h"

using user_t = std::string;
using notification_id = uint32_t;

struct UserQueues
{
    std::mutex received_mutex;
    std::shared_ptr<std::map<notification_id,notification>> received_notifications = std::make_shared<std::map<notification_id,notification>>();
    std::shared_ptr<SafeQueue<std::pair<user_t,notification_id>>> consumer_queue = std::make_shared<SafeQueue<std::pair<user_t,notification_id>>>();
};

class NotificationController
{
    std::mutex data_mutex;
    std::map<user_t,UserQueues> data;

    UserController* user_controller;

    void lockReceivedList(user_t user);
    void unlockReceivedList(user_t user);

public:

    SafeQueue<std::pair<user_t,notification>> producerQueue;

    NotificationController(UserController* u) : user_controller(u) {};

    std::pair<user_t,notification> consume(user_t user);
    void produce(user_t user, notification noti);

    void consumerThread(user_t user);
    void producerThread();
};

#endif