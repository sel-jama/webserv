#ifndef SCLIENT_HPP
#define SCLIENT_HPP
#include "WebServer.hpp"

class client
{
    private:
        // client(const client &);
        const client &operator=(const client &);

    public:
        // client(int fd, fd_set &fd_r, int &maxfd)
        client();
        ~client();
        //vars
        int ssocket;
        struct sockaddr_in cdata_socket;
        struct timeval clientTime;
        int state;
};



#endif