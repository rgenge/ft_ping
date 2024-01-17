#include "ping.h"

struct  info g_info = {0};

void interrupt()
{
    printf("\n--- %s ping statistics ---", g_info.dest);
    exit(0);
}

void get_addr(char *ip, void *addr)
{
    struct addrinfo *result;

    if (getaddrinfo(ip, NULL, NULL, &result)) {
        fprintf(stderr, "ft_ping: %s: Name or service not known\n", ip);
        exit(2);
    }
    for (struct addrinfo *node = result ; node && node->ai_next ; node = node->ai_next)
    {
        if (((struct sockaddr_in *)node->ai_addr)->sin_addr.s_addr)
        {
            ft_memcpy(addr, node->ai_addr, sizeof(struct sockaddr_in));
            break ;
        }
    }
    //   memset(addr, 0, sizeof(struct sockaddr_in));
    // addr->sin_family = AF_INET;
    // addr->sin_port = htons(0);  // Use any available port
    // inet_pton(AF_INET, ip, &(addr->sin_addr));
    freeaddrinfo(result);
}
void ping(char *ip)
{
    struct sockaddr_in addr = {0};
    get_addr(ip, &addr);
    char buf[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, buf, sizeof(buf));
 //   printf("%s", buf);

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        fprintf(stderr, "Error: Could not connect to the IP provided\n");
        exit(2);
    }
    printf("PING %s (%s) 56(84) bytes of data.\n", ip, buf);

    struct timeval get_time = getnow();
    for (int i = 0; i < 16; i++)
    {
        usleep(100000);
        struct timeval get_finish = getnow();
        printf("%ld\n", get_finish.tv_sec- get_time.tv_sec);
    }





 //   while(1){printf("o");}
}
int main (int argc, char **argv){

    char flag;
    if (argc != 2 && argc != 3)
    {
        fprintf(stderr, "Your must have 2 or 3 arguments");
        exit(1);
    }
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-' && argv[i][2] == '\0')
            flag = argv[i][1];            
    }
    if (flag == 'h')
    {
        printf("Usage\n\tping [options] <destination>\n\nOptions:\n"
        "\t<destination>      dns name or ip address\n"
        "\t-h\t        print help and exit\n"
        "\t-n\t        do dns name resolution\n");
    }
    else
    {
        g_info.dest = argv[1];
        g_info.start = getnow();
        g_info.min = 100000;
        signal(SIGINT, interrupt);
        ping(g_info.dest);

    }

}