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

// Errors & signals
# include <signal.h>
# include <errno.h>

struct info {
    unsigned int    sent;
    unsigned int    received;
    char *          dest;
    struct timeval  start;
    unsigned int    min;
    unsigned int    max;
    unsigned int    sum;
    unsigned int    sqsum;
};

struct timeval getnow (void);
void	*ft_memcpy(void *dest, const void *src, size_t size);
#endif
