#include "server.hpp"

void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
	  return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char** argv) {
	std::cout << "[] Starting webserver on port " << DEFAULT_PORT << std::endl;
	struct sockaddr_storage incomingAddress;
	socklen_t addressSize;
	struct addrinfo hints, *res;

	int sockfd, newSockfd, status;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // Either IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP connections

	if ((status = getaddrinfo("0.0.0.0", DEFAULT_PORT, &hints, &res)) == -1) {
		throw gai_strerror(status);
	}
	std::cout << "[] Opened new socket on " << res->ai_addr << std::endl;

	// Making a socket, bind it and listen on it

	if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) { // Socket file descriptor
		throw "An error occured while getting the socket file descriptor";
	}
	std::cout << "[] Got the socket file descriptor" << std::endl;

	try {
		if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
			close(sockfd);
			throw "Couldn't bind socket to port, try again"; // Bind socket to port
		}
	} catch (const char* exc) {
		std::cerr << exc << std::endl;
		std::cerr << strerror(errno) << std::endl;
	}
	freeaddrinfo(res);
	char* buffer = new char[MAX_BUFFER_SIZE];

	if (listen(sockfd, BACKLOG) == -1)
		throw "Invalid socket";

	int i = 0;
	while (true) {
		std::cout << ++i << std::endl;

		addressSize = sizeof(incomingAddress);
		newSockfd = accept(sockfd, (struct sockaddr*)&incomingAddress, &addressSize);

		std::cout << "Acceptare " << i << std::endl;

		if (newSockfd == -1) {
			throw "An error occured when accepting an incoming connection";
		}
		std::cout << "Incoming connection accepted" << std::endl;

		pid_t forkStatus;
		if ((forkStatus = fork()) == -1)
			throw strerror(errno);

		if (forkStatus > 0) { // Child process
			std::cout << "This is coming from the child" << std::endl;
			close(sockfd); // Doesn't need listener socket

			// Now we have a new socket file descriptor, newSockfd that will be used for receiving requests
			if (recv(newSockfd, buffer, MAX_BUFFER_SIZE*sizeof(char), 0) == -1) {
				throw "Error when receiving request";
			}
			std::cout << "Request: " << buffer << std::endl;

			char* incomingAddressString = new char[INET6_ADDRSTRLEN];
			inet_ntop(incomingAddress.ss_family, get_in_addr((struct sockaddr *)&incomingAddress),
				incomingAddressString, sizeof incomingAddressString);
			std::cout << "From address: " << incomingAddressString << std::endl;

			delete[] incomingAddressString;
			close(newSockfd);
			exit(EXIT_SUCCESS);
		} 
		// Parent process
		std::cout << "[x]This is coming from the parent, closing the new connection" << std::endl;
		close(newSockfd);
	}

	delete[] buffer;
	close(sockfd);
	std::cout << "[] Closing server" << std::endl;

	return 0;
}