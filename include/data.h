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
    const char* payload;
};

struct notification
{
    uint32_t id;
    uint32_t timestamp;
};