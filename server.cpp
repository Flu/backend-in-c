#include "server.hpp"

int main(int argc, char** argv) {
	std::cout << "[] Starting webserver on port 2000" << std::endl;
	struct sockaddr_storage incomingAddress;
	socklen_t addressSize;
	struct addrinfo hints, *res;

	int sockfd, newSockfd, status;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // Either IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP connections

	if ((status = getaddrinfo("192.168.1.30", "2000", &hints, &res)) == -1) {
		throw gai_strerror(status);
	}
	std::cout << "[] Opened new socket on " << res->ai_addr << std::endl;

	// Making a socket, bind it and listen on it

	if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) { // Socket file descriptor
		throw "An error occured while getting the socket file descriptor";
	}
	std::cout << "[] Got the socket file descriptor" << std::endl;

	bind(sockfd, res->ai_addr, res->ai_addrlen); // Bind socket to port

	char* buffer = new char[MAX_BUFFER_SIZE];

	while (true) {
		listen(sockfd, BACKLOG);

		addressSize = sizeof(incomingAddress);
		newSockfd = accept(sockfd, (struct sockaddr*)&incomingAddress, &addressSize);
		if (newSockfd == -1) {
			throw "An error occured when accepting an incoming connection";
		}
		std::cout << "Incoming connection accepted" << std::endl;
	
		// Now we have a new socket file descriptor, newSockfd that will be used for receiving requests
		if (recv(newSockfd, buffer, MAX_BUFFER_SIZE*sizeof(char), 0) == -1) {
			throw "Error when receiving request";
		}
		std::cout << "Request: " << buffer << std::endl;
		std::cout << "From address: " << (struct sockaddr*)&incomingAddress << std::endl;
	}

	delete[] buffer;
	freeaddrinfo(res);
	std::cout << "[] Closing server" << std::endl;
	return 0;
}