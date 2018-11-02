#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define DEFAULT_PORT 2000
#define BACKLOG 20

int main(int argc, char** argv) {
	printf("[] Starting webserver on port 2000 \n");
	struct sockaddr_storage incomingAddress;
	socklen_t addressSize;
	struct addrinfo hints, *res;

	int sockfd, newSockfd, status;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // Either IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP connections

	if ((status = getaddrinfo("192.168.1.30", "2000", &hints, &res)) == -1) {
		fprintf(stderr, "An error occured when looking for this IP address:\n %s \n", gai_strerror(status));
		fflush(stderr);
		return 1;
	}
	printf("[] Opened new socket on %s\n", res->ai_addr);

	// Making a socket, bind it and listen on it

	if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) { // Socket file descriptor
		fprintf(stderr, "An error occured while getting the socket file descriptor\n");
		return 2;
	}
	printf("[] Got the socket file descriptor\n");

	bind(sockfd, res->ai_addr, res->ai_addrlen); // Bind socket to port
	listen(sockfd, BACKLOG);

	addressSize = sizeof(incomingAddress);
	newSockfd = accept(sockfd, (struct sockaddr*)&incomingAddress, &addressSize);
	if (newSockfd == -1) {
		fprintf(stderr, "An error occured when accepting an incoming connection\n");
		return 3;
	}
	printf("Incoming connection accepted\n");
	
	// Now we have a new socket file descriptor, newSockfd that will be used for receiving requests

	char* buffer = (char*)malloc(50*sizeof(char));
	if (recv(newSockfd, buffer, 50*sizeof(char), 0) == -1) {
		fprintf(stderr, "Error when receiving request\n");
		return 4;
	}
	printf("Request: %s\n", buffer);

	freeaddrinfo(res);
	free(buffer);
	printf("[] Closing server\n");
	return 0;
}