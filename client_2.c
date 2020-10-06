// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	 8080
#define MAX_LINE 256

// Driver code
int main() {
    int s;
    struct sockaddr_in	 sin;
    char buf[MAX_LINE];

    // Creating socket file descriptor
    if ( (s = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&sin, 0, sizeof(sin));

    // Filling server information
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    sin.sin_addr.s_addr = INADDR_ANY;

    int n, len;
    while(1) {
        if(fgets(buf, sizeof(buf), stdin)) {
            sendto(s, (const char *) buf, strlen(buf),
                   MSG_CONFIRM, (const struct sockaddr *) &sin,
                   sizeof(sin));
            printf("Message sent.\n");
        }
        n = recvfrom(s, (char *) buf, MAX_LINE, MSG_WAITALL, (struct sockaddr *) &sin, &len);

        buf[n] = '\0';
        printf("received: ");
        fputs(buf, stdout);
    }
}
