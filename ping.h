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
};
struct packet {
    uint8_t     type;         
    uint8_t     code;   
    uint16_t    checksum;   
    uint16_t    identifier;   
    uint16_t    sequence_number;
    char        time[8];       
    char        data[48];
};

typedef struct _iphdr
{
    unsigned int    h_len : 4;        // Length of the header
    unsigned int    version : 4;      // Version of IP
    unsigned char   tos;            // Type of service
    unsigned short  total_len;      // Total length of the packet
    unsigned short  ident;          // Unique identifier
    unsigned short  frag_and_flags; // Flags
    unsigned char   ttl;            // Time to live
    unsigned char   proto;          // Protocol (TCP, UDP etc)
    unsigned short  checksum;       // IP checksum  
    unsigned int    sourceIP;
    unsigned int    destIP;
} IpHeader;

struct icmp_echo {
    // header
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t identifier;
    uint16_t seq;

    // data
    double  sending_ts;
    char    magic[11];
};
void            decode_icmp_header(char *buf);
struct timeval  getnow (void);
void            *ft_memcpy(void *dest, const void *src, size_t size);
uint16_t        checksum(uint16_t *data, size_t size);
void            settime (void *time);
unsigned int timediff (struct timeval old, struct timeval new);
unsigned int    min (unsigned int a, unsigned int b);
unsigned int    max (unsigned int a, unsigned int b);

#endif
