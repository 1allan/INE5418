#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "requests.h"

int open_connection(int port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_addr = {.s_addr = inet_addr("192.168.1.178")},
        .sin_port = htons(port)
    };
    int result = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
    if (result == -1) { return -1; }
    return sockfd;
}

payload_t get_line(int sockfd, int index)
{
    payload_t request = { .operation = 1 };
    write(sockfd, &request, sizeof(request));
    read(sockfd, &request, sizeof(request));
    return request;
}

int main()
{
    int sockfd = open_connection(9734);
    
    if(sockfd == -1) {
        perror("!!!connection failed!!!\n");
        exit(1);
    }
    
    payload_t a = get_line(sockfd, 0);
    printf("%s\n", a.body);
    
    close(sockfd);
    exit(0);
}
