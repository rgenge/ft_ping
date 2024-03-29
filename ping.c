#include "ping.h"

struct  info g_info = {0};

void interrupt()
{
    printf("\n--- %s ping statistics ---", g_info.dest);
    exit(0);
}

void decode_icmp_header(char *buf)
{
    IpHeader       *iphdr = NULL;

    iphdr = (IpHeader *)buf;
    int received_ttl = iphdr->ttl;
    printf("TTL value: %d\n", received_ttl);
}

int receive_packet(int sockfd, struct sockaddr_in addr)
{
    char buffer[1500];
    printf("sockfd: %d\n", sockfd);
    // receive another packet
    struct sockaddr_in from;
    unsigned int fromlen = sizeof(from);
    int bytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&from, &fromlen);
        printf("bytes: %d\n", bytes);
    if (bytes == -1) {
        // normal return when timeout
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return 0;
        }
        return -1;
    }
    struct icmp_echo* icmp = (struct icmp_echo*)(buffer + 20);

    // check type
    if (icmp->type != 0 || icmp->code != 0) {
        printf("entroutype %d\n",icmp->type );
        return 0;
    }

    // match identifier
    if (ntohs(icmp->identifier) != g_info.id) {
        printf("entrouident\n");
        return 0;
    }
    // print info
    printf("%s seq=%d ms\n",
        inet_ntoa(addr.sin_addr),
        ntohs(icmp->seq)
    );
    decode_icmp_header(buffer);
    return 0;
}
void send_packet(int sockfd, struct sockaddr_in addr)
{
    struct icmp_echo packet;
    bzero(&packet, sizeof(packet));

    packet.type = 8;
    packet.identifier = htons(g_info.id);
    packet.code = 0;
    packet.seq = g_info.seq;
    packet.checksum = 0;
    packet.checksum = checksum((uint16_t *)&packet, sizeof(packet));
    int ret = sendto(sockfd, &packet, 64, 0, (struct sockaddr *)&addr, sizeof(addr));

    if (ret > 0)
    {
        g_info.sent++;
        printf("sent %d", ret);
    }


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
    struct sockaddr_in addr_2 = {0};
    get_addr(ip, &addr);
     get_addr(ip, &addr_2);
    char buf[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, buf, sizeof(buf));
 //   printf("%s", buf);

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        fprintf(stderr, "Error: Could not connect to the IP provided\n");
        exit(2);
    }
    g_info.ip = ip;
    struct timeval get_time = getnow();
    g_info.seq = 1;
    g_info.id = getpid();
    for (int i = 0; i < 8; i++)
    {
        send_packet(sockfd, addr);
        usleep(1000000);
        receive_packet(sockfd, addr_2);
        struct timeval get_finish = getnow();
        printf("%ld\n", get_finish.tv_sec- get_time.tv_sec);
        g_info.seq++;
    }
}
int main (int argc, char **argv){

    char flag;
    flag = ' ';
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