#include "ping.h"

struct  info g_info = {0};

void get_hostname(const char *ip_address) {
    struct sockaddr_in sa;
    char host[NI_MAXHOST];

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(ip_address);

    if (getnameinfo((struct sockaddr *)&sa, sizeof(sa), host, NI_MAXHOST, NULL, 0, 0) == 0) {
        printf("%s %s", ip_address, host);
    } else {
        printf(" %s ", ip_address);
    }
}

void interrupt()
{
    printf("\n--- %s ping statistics ---", g_info.dest);
    printf("%u packets transmitted, %u received, %u%% packet loss, time %ums\n",
        g_info.sent, g_info.received, 100 - (100 * g_info.received / g_info.sent), timediff(g_info.start, getnow()) / 1000);
    
    if (g_info.received) {
        unsigned int avg = g_info.sum / g_info.received;
        unsigned int mdev = sqrt((g_info.squaresum / g_info.received) - (avg * avg));

        printf("rtt min/avg/max/mdev = %u.%u/%u.%u/%u.%u/%u.%u ms\n",
            g_info.min / 1000, g_info.min % 1000, avg / 1000, avg % 1000,
            g_info.max / 1000, g_info.max % 1000, mdev / 1000, mdev % 1000);
    }
    exit(0);
}

void decode_icmp_header(char *buf)
{
    IpHeader       *iphdr = NULL;

    iphdr = (IpHeader *)buf;
    int received_ttl = iphdr->ttl;
    printf("TTL=%d ", received_ttl);
}

int receive_packet(int sockfd, struct sockaddr_in addr)
{
    char buffer[1500];
    struct sockaddr_in from;
    unsigned int fromlen = sizeof(from);
    int bytes;
    for (int i = 0; i < 5; i++){
    
        bytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&from, &fromlen);
        if (bytes != -1)
        {
            break;
        }
        usleep(1000);
    }
    if (bytes == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return 0;
        }
        return -1;
    }
    struct icmp_echo* icmp = (struct icmp_echo*)(buffer + 20);
    if (icmp->type != 0 || icmp->code != 0) {
        return 0;
    }
    if (ntohs(icmp->identifier) != g_info.id) {
        return 0;
    }
    get_hostname(inet_ntoa(addr.sin_addr));
    printf(" icmp_seq=%d ",g_info.seq);
    decode_icmp_header(buffer);
    g_info.received++;
    return 0;
}

int send_packet(int sockfd, struct sockaddr_in addr)
{
    struct icmp_echo packet;
    bzero(&packet, sizeof(packet));
    int flag = 0;

    packet.type = 8;
    packet.identifier = htons(g_info.id);
    packet.code = 0;
    packet.seq = g_info.seq;
    packet.checksum = 0;
    packet.checksum = checksum((uint16_t *)&packet, sizeof(packet));
    int ret = sendto(sockfd, &packet, 64, 0, (struct sockaddr *)&addr, sizeof(addr));
    if (ret > 0)
    {
        printf("%d bytes from: ", ret);
        flag = 1;
    }
    g_info.sent++;
    return flag;
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
    freeaddrinfo(result);
}


void ping(char *ip)
{
    struct sockaddr_in addr = {0};
    get_addr(ip, &addr);
    char buf[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, buf, sizeof(buf));
    printf("\nPING:%s\n", inet_ntoa(addr.sin_addr));
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        fprintf(stderr, "Error: Could not connect to the IP provided\n");
        exit(2);
    }
    g_info.ip = ip;
    struct timeval get_time;
    g_info.seq = 1;
    g_info.id = getpid();
    unsigned int triptime;
    int send_flag;
    for (int i = 0; i < 8; i++)
    {
        get_time = getnow();     
        send_flag = send_packet(sockfd, addr);
        if(send_flag == 1)
        {
            receive_packet(sockfd, addr);
        }      
        triptime = timediff(get_time, getnow());
        printf("time: %d.%d\n", triptime / 1000, triptime % 100);
        g_info.min = min(g_info.min, triptime);
        g_info.max = max(g_info.max, triptime);
        g_info.sum += triptime;
        g_info.squaresum += triptime * triptime;
        g_info.seq++;
        usleep(1000000);
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
        if ((argc == 3 ) && argv[i][0] == '?' && argv[i][1] == '\0' && argv[i - 1][0] == '-' && argv[i - 1][1] == 'v')
            flag = 'h';          
    }
    if (flag == 'h' || (argc >2 && argv[2][0] == '-' && argv[2][1] == '?' && argv[2][2] == '\0'))
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
        g_info.min = 50000;
        signal(SIGINT, interrupt);
        ping(g_info.dest);
    }
}