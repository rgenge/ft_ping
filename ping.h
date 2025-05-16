#ifndef FT_PING_H
# define FT_PING_H

# define DATA "0123456789012345678901234567890123456789012345678912345"
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
    unsigned        sent;
    unsigned int    received;
    char *          dest;
    struct timeval  start;
    unsigned int    min;
    unsigned int    seq;
    unsigned int    prev;
    unsigned int    max;
    unsigned int    sum;
    unsigned int    squaresum;
    unsigned int    id;
    char *          ip;
    unsigned int    v_flag;
	char *          hostname;
};

typedef struct _iphdr
{
    unsigned char   ttl;
} IpHeader;

struct icmp_echo {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t identifier;
    uint16_t seq;
    char     time[8];
    char     data[48];
};

void            decode_icmp_header(char *buf);
struct timeval  getnow (void);
uint16_t        checksum(uint16_t *data, size_t size);
void            settime (void *time);
unsigned int timediff (struct timeval old, struct timeval new);
unsigned int    min (unsigned int a, unsigned int b);
unsigned int    max (unsigned int a, unsigned int b);

#endif
