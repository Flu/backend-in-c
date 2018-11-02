#pragma once
#include <iostream>
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
#define MAX_BUFFER_SIZE 1000

class Server {
	sockaddr_storage incomingAddress;
};