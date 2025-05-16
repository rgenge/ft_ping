#include "ping.h"

// Current time
struct timeval getnow (void)
{
    struct timeval now;

    gettimeofday(&now, 0);
    return now;
}

uint16_t checksum(uint16_t *data, size_t size) {
	uint32_t sum = 0;

    for (size_t i = 0; i < size / 2; i++) {
        sum += data[i];
    }
    sum = (sum >> 16) + (sum & 0xffff);

    return ~sum;
}

void settime (void *time)
{
    struct timeval ptr;
    gettimeofday(&ptr, 0);
    memcpy(time, &ptr.tv_sec, 4);
}

unsigned int min (unsigned int a, unsigned int b)
{
    if (a < b)
        return a;
    else
        return b;
}

unsigned int max (unsigned int a, unsigned int b)
{
    if (a > b)
        return a;
    else
        return b;
}

unsigned int timediff (struct timeval old, struct timeval new)
{
    return (new.tv_sec - old.tv_sec) * 1000000 + new.tv_usec - old.tv_usec;
}
