#ifndef DATA_H
#define DATA_H

#include <cstdint>
#include <sys/types.h>

#define CMD 0
#define DATA 1
struct packet
{
    uint16_t type;
    uint16_t seqn;
    uint16_t length;
    time_t timestamp;
    char payload[256];
};

struct notification
{
    uint32_t id;
    uint32_t timestamp;
    uint16_t length;
    uint16_t pending;
    char msg[256];
};

#endif