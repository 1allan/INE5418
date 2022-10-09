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
#include <pthread.h>
#include "requests.h"
#include "file.c"

pthread_mutex_t lock;

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

void log_request(payload_t *request)
{
	pthread_t id = pthread_self();
	printf("[%ld] Operation: %s\n", id, request->operation);
	printf("[%ld] Line: %d\n", id, request->body.line);
	printf("[%ld] Content: %s\n", id, request->body.content);
}

void* handle_request(void *args)
{
	socklen_t socket = *((socklen_t*)args);
	payload_t request;

	while (1) {
		read(socket, &request, sizeof(request));

		log_request(&request);

		if (strcmp(request.operation, "put") == 0) {
			pthread_mutex_lock(&lock);
			put(request.body.line, request.body.content);
			pthread_mutex_unlock(&lock);
		} else if (strcmp(request.operation, "get") == 0) {
			get(request.body.line, request.body.content);
		} else {
			close(socket);
			write(socket, &request, sizeof(request));
			break;
		}
		write(socket, &request, sizeof(request));
	}
	return NULL;
}

int main()
{
	struct sockaddr_in client_address;
	int client_sockfd;
	socklen_t client_len;
	pthread_t thread_id;

	int server_sockfd = open_server_socket(9734, 100000);

	while(1) {
		printf("[%d] server waiting\n", getpid());

		client_len = sizeof(client_address);
		client_sockfd = accept(
			server_sockfd,
			(struct sockaddr *)&client_address,
			&client_len
		);

		printf("socket in main: %d\n", client_sockfd);

		pthread_create(&thread_id, NULL, handle_request, (void *) &client_sockfd);
	}
    pthread_mutex_destroy(&lock);
}
