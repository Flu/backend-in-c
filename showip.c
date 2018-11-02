/*
Showip will show the ip of any host
Fluturel Adrian, 2018
*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

typedef struct addrinfo addrinfo;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr_in6 sockaddr_in6;

int printIPs(const addrinfo *head, char *ipstr) {
	const addrinfo *current = head;
	void *addr;
	char *ipVersion;

	while (current) {
		if (current->ai_family == AF_INET) {
			sockaddr_in *v4 = (sockaddr_in*)current->ai_addr;
			addr = &(v4->sin_addr);
			ipVersion = "IPv4";
		}
		if (current->ai_family == AF_INET6) {
			sockaddr_in6 *v6 = (sockaddr_in6*)current->ai_addr;
			addr = &(v6->sin6_addr);
			ipVersion = "IPv6";
		}
		inet_ntop(current->ai_family, addr, ipstr, INET6_ADDRSTRLEN);
		printf("\t%s: %s\n", ipVersion, ipstr);
		current = current->ai_next;
	}
}

int main(int argc, char** argv) {
	addrinfo *res;
	addrinfo hints;
	int status;

	char ipstr[INET6_ADDRSTRLEN];

	if (argc != 2) {
		fprintf(stderr, "Usage: showip [hostname]");
		fflush(stderr);
		return 1;
	}

	argv++;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo(*argv, NULL, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo returned an error: %s \n", gai_strerror(status));
		fflush(stderr);
		return 2;
	}
	printf("Host %s: \n", *argv);
	printIPs(res, ipstr);

	freeaddrinfo(res);
	return 0;
}