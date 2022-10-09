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

payload_t send_(int sockfd, payload_t payload)
{
    write(sockfd, &payload, sizeof(payload));
    read(sockfd, &payload, sizeof(payload));
    return payload;
}

payload_t parse_input(char input[1024])
{
    char *content = strtok(input, ":");
    content = strtok(NULL, ":");
    char op[4];
    char number[5];
    strncpy(op, input, 3);
    strncpy(number, strtok(&input[3], ":"), 4);
    
    payload_t payload = {
        .body = {
            .line = strtol(number, '\0', 10),
        }
    };
    strcpy(payload.operation, op);
    strcpy(payload.body.content, content);
    return payload;
}

int main()
{
    int sockfd = open_connection(9734);
    if(sockfd == -1) {
        perror("!!!connection failed!!!\n");
        exit(1);
    }

    char input[1024];
    while (1) {
        printf("> ");
        scanf("%s", input);
        payload_t request = parse_input(input);
        payload_t response = send_(sockfd, request);
        printf("%s\n", response.body.content);
        if (strcmp(request.operation, "exi") == 0) {
            break;
        }
    }
    close(sockfd);

    exit(0);
}
