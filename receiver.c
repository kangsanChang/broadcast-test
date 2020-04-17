#include <stdio.h>
#include <netinet/in.h>
#include <signal.h>
#include <time.h>
#include <zconf.h>
#include <stdlib.h>

#define MAXBUF 256

long long getCurrentTimestamp() {
    /* Example of timestamp in microsecond. */
    struct timeval timer_usec;
    long long int timestamp_usec; /* timestamp in microsecond */
    if (!gettimeofday(&timer_usec, NULL)) {
        timestamp_usec = ((long long int) timer_usec.tv_sec) * 1000000ll +
                         (long long int) timer_usec.tv_usec;
    } else {
        timestamp_usec = -1;
    }
    return timestamp_usec;
}

int main(int argc, char** argv) {
    int ssock;
    int clen;
    struct sockaddr_in client_addr, server_addr;
    char buf[MAXBUF];
    char buf2[MAXBUF];

    if((ssock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("Socket Error : ");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(3317);

    if (bind(ssock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind error : ");
        exit(1);
    }
    clen = sizeof(client_addr);

    recvfrom(ssock, (void *) buf, MAXBUF, 0, (struct sockaddr *) &client_addr, &clen);
    long long now = getCurrentTimestamp();
    sprintf(buf2, "%lld\0", now);
    printf("Current time: %s\n", buf2);

    long long received_time;
    received_time = atoll(buf);
    ctime(&received_time);

    printf("Received message from broadcaster %s\n", buf);

    long long diff = now - received_time;
    printf("DIFF : %lld\n", diff);

    char result[MAXBUF];
    sprintf(result, "%s, %lld, %lld, %lld\n", argv[1], received_time, now, diff);
    strcpy(buf, result);

    sendto(ssock, (void *) result, MAXBUF, 0, (struct sockaddr *) &client_addr, sizeof(client_addr));

    close(ssock);
    return 0;
}
