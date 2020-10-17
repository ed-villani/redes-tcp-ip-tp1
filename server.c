//
// Created by Arthur Raposo on 27/08/2020.
//

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

#define SERVER_PORT 54321
#define MSG_CONFIRM 0

int main(int argc, char *argv[])
{
    struct sockaddr_in sin, cliaddr;
    unsigned int MAX_LINE;
    int len, n;
    int s;

    if (argc == 2)
    {
        MAX_LINE = atoi(argv[1]);
    }
    else
    {
        fprintf(stderr, "arg error");
        exit(1);
    }

    char buf[MAX_LINE];
    /*build address data structure*/
    bzero((char *)&sin, sizeof(sin));
    bzero((char *)&cliaddr, sizeof(cliaddr));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(SERVER_PORT);

    /*setup passive open*/
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("simplex-talk: socket");
        exit(1);
    }
    if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0)
    {
        perror("simplex-talk: bind");
        exit(1);
    }

    len = sizeof(cliaddr); //len is value/resuslt

    while (n = recvfrom(s, (char *)buf, MAX_LINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len))
    {
        buf[n] = '\0';
        printf("Client : %s\n", buf);
        printf("Message sent back.\n");
        sendto(s, (const char *)buf, strlen(buf), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
    }
}
