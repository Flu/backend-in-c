#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define DEFAULT_PORT "2000"
#define BACKLOG 20
#define MAX_BUFFER_SIZE 1000

class Server {
	public:
	explicit Server(const std::string&);
};