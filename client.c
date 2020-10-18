#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <arpa/inet.h>
#include "custom_file.h"

#define SERVER_PORT 54321
// #define MAX_LINE 10

int rand_str(char *dest, size_t length) {
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (length-- > 0) {
        size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
    return 1;
}

int main(int argc, char *argv[])
{
    struct hostent *hp;
    struct sockaddr_in sin;
    char *file_size;
    char *host;
    int MAX_LINE;
    long unsigned int iteration_numbers = 1000;
    int s;
    int n, len;
    struct timeval stop, start;
    FILE *fp;

    if (argc > 1)
    {
        host = argv[1];
        file_size = argv[2];
        MAX_LINE = atoi(file_size);
    }
    else
    {
        fprintf(stderr, "usage: simplex-talk host\n");
        exit(1);
    }
    char buf[MAX_LINE];
    /* translate host name into peer’s IP address */
    hp = gethostbyname(host);
    if (!hp)
    {
        fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
        exit(1);
    }
    inet_aton(host, &sin.sin_addr);
    /* build address data structure */
    //bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
    sin.sin_port = htons(SERVER_PORT);
    /* active open */
    if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("simplex-talk: socket");
        exit(1);
    }
    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        perror("simplex-talk: connect");
        close(s);
        exit(1);
    }
    // long int k = 0;
    long int i = 0;
    while (1)
    {   

        // fp = createOrFindFile(file_size);
        if (i > iteration_numbers - 1)
            break;
        // if (atoi(file_size) < MAX_LINE || i % (atoi(file_size)/MAX_LINE) == 0)
        // {
        //     fp = createOrFindFile(file_size);
        //     k = k + 1;
        // }
        // if (fgets(buf, sizeof(buf), fp))
        if (rand_str(buf, MAX_LINE+1))
        {   
            printf("%ld,", i);
            printf("%ld,", sizeof(buf));
            // printf("%ld,", k);
            gettimeofday(&start, NULL);
            sleep(0.5);
            sendto(s, (const char *)buf, strlen(buf),
                   MSG_CONFIRM, (const struct sockaddr *)&sin,
                   sizeof(sin));
            i = i + 1;
            // printf("Message sent.\n");
        }
        n = recvfrom(s, (char *)buf, MAX_LINE, MSG_WAITALL, (struct sockaddr *)&sin, &len); 
        if (n)
        {
            gettimeofday(&stop, NULL);
            long unsigned delta = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
            printf("%lu\n", delta);
            // printf("\ntook %lu us\n", delta);
        }
        buf[n] = '\0';
        // printf("received: ");
        // fputs(buf, stdout);
        // printf("\n");
        // fclose(fp);
    }
}
