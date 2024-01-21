#ifndef FT_PING_H
# define FT_PING_H

// Std
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>

// Time, types, math
# include <sys/time.h>
# include <stdint.h>
# include <math.h>

// Networking
# include <netdb.h>
# include <arpa/inet.h>
# include <linux/if_packet.h>
# include <netinet/ip_icmp.h>

// Errors & signals
# include <signal.h>
# include <errno.h>

struct info {
    unsigned int    sent;
    unsigned int    received;
    char *          dest;
    struct timeval  start;
    unsigned int    min;
    unsigned int    seq;
    unsigned int    max;
    unsigned int    sum;
    unsigned int    sqsum;
    unsigned int    id;
};
struct packet {
    uint8_t     type;           // 8 for request, 0 for reply
    uint8_t     code;           // Always 0
    uint16_t    checksum;       // Computed total for checks
    uint16_t    identifier;     // Unique ID of current ping "loop" (always 0 here)
    uint16_t    sequence_number;// Unique ID of ping in loop (starts at 1, increments)
    char        time[8];        // Timestamp as data optimization
    char        data[48];       // Data (static)
};

struct icmp_echo {
    // header
    uint8_t type;
    uint8_t code;
    uint16_t checksum;

    uint16_t identifier;
    uint16_t seq;

    // data
    double sending_ts;
    char magic[11];
};

struct timeval getnow (void);
void	*ft_memcpy(void *dest, const void *src, size_t size);
uint16_t checksum(uint16_t *data, size_t size);
void settime (void *time);

#endif
