#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <time.h>
#include <string.h>

#define MAXBUF 256

//void signal_handler(int ssock, struct sockaddr* client_addr);

long long getCurrentTimestamp() {
    /* Example of timestamp in microsecond. */
    struct timeval timer_usec;
    long long int timestamp_usec; /* timestamp in microsecond */
    if (!gettimeofday(&timer_usec, NULL)) {
        timestamp_usec = ((long long int) timer_usec.tv_sec) * 1000000ll +
                         (long long int) timer_usec.tv_usec;
    }
    else {
        timestamp_usec = -1;
    }
    return timestamp_usec;
}

int main() {
    int ssock;
    int clen;
    struct sockaddr_in client_addr, server_addr;
    char buf[MAXBUF];

    long long now = getCurrentTimestamp();
    sprintf(buf, "%lld\0", now);

    if((ssock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))<0){
        perror("socket error: ");
        exit(1);
    }
    int enable_broadcast = 1;
    setsockopt(ssock, SOL_SOCKET, SO_BROADCAST, (int*)&enable_broadcast, sizeof(enable_broadcast));

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_BROADCAST;
    server_addr.sin_port = htons(3317);

    sendto(ssock, (void*) buf, MAXBUF, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    clen = sizeof(client_addr);

    while(1) {

        recvfrom(ssock, (void *) buf, MAXBUF, 0, (struct sockaddr *) &client_addr, &clen);
        printf("Receive message : %s\n", buf);

    }
    close(ssock);
    return 0;
}
