#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "requests.h"
#include "file.c"

int open_server_socket(int port, int max_connections)
{
	struct sockaddr_in const address = {
		.sin_family = AF_INET,
		.sin_addr = {.s_addr = htonl(INADDR_ANY)},
		.sin_port = htons(port)
	};
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	bind(socket_fd, (struct sockaddr *)&address, sizeof(address));
	int result = listen(socket_fd, max_connections);
	if (result == -1) { return -1; }
	return socket_fd;
}

int main()
{
	struct sockaddr_in client_address;
	int client_sockfd;
	int client_len;
	payload_t request;
	
	int server_sockfd = open_server_socket(9734, 100000);

	while(1) {
		printf("[%d] server waiting\n", getpid());
		
		client_len = sizeof(client_address);
		client_sockfd = accept(
			server_sockfd,
			(struct sockaddr *)&client_address,
			&client_len
		);
		
		read(client_sockfd, &request, sizeof(request));
		
		printf("operation: %s\n", request.operation);
		printf("line: %d\n", request.body.line);
		printf("content: %s\n", request.body.content);
		printf("%d", strcmp(request.operation, "put"));
		
		if (strcmp(request.operation, "put") == 0) {
			put(request.body.line, request.body.content);
		} else if (strcmp(request.operation, "get") == 0) {
			get(request.body.line, request.body.content);
		}
		
		write(client_sockfd, &request, sizeof(request));
		
		close(client_sockfd);
	}
}
