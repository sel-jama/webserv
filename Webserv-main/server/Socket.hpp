#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <signal.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstdlib>
#include <stdio.h> 

#define SERVER_PORT 18000

#define MAXLINE 4096
#define SA struct sockaddr

using namespace std;


class Socket{
    private:
        int sock;
        struct sockaddr_in address;

    public:
        // Socket(int domain, int type, int protocol, int port, u_long ip);
        void clientConnect(char **av);
        void serverConnect();
};

#endif