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

#define SERVER_PORT 54321
#define MAX_LINE 256

int main(int argc, char * argv[]) {
    struct hostent *hp;
    struct sockaddr_in sin;
    char *host;
    char buf[MAX_LINE];
    int s;
    int n, len;
    struct timeval stop, start;

    if (argc == 2) {
        host = argv[1];
    } else {
        fprintf(stderr, "usage: simplex-talk host\n");
        exit(1);
    }
    /* translate host name into peer’s IP address */
    hp = gethostbyname(host);
    if (!hp) {
        fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
        exit(1);
    }
    inet_aton(host, &sin.sin_addr);
    /* build address data structure */
    //bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    bcopy(hp->h_addr, (char *) &sin.sin_addr, hp->h_length);
    sin.sin_port = htons(SERVER_PORT);
    /* active open */
    if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("simplex-talk: socket");
        exit(1);
    }
    if (connect(s, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("simplex-talk: connect");
        close(s);
        exit(1);
    }

    while(1) {
        if(fgets(buf, sizeof(buf), stdin)) {
            gettimeofday(&start, NULL);
            sendto(s, (const char *) buf, strlen(buf),
                   MSG_CONFIRM, (const struct sockaddr *) &sin,
                   sizeof(sin));
            printf("Message sent.\n");
        }
        n = recvfrom(s, (char *) buf, MAX_LINE, MSG_WAITALL, (struct sockaddr *) &sin, &len);
        if (n>0){
            gettimeofday(&stop, NULL);
            printf("\ntook %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
        }
        buf[n] = '\0';
        printf("received: ");
        fputs(buf, stdout);
    }
}
