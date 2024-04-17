#ifndef SCLIENT_HPP
#define SCLIENT_HPP
#include "WebServer.hpp"

class client
{
    private:

    public:
        client(const client &){}
        const client &operator=(const client &);
        // client(int fd, fd_set &fd_r, int &maxfd)
        client();
        ~client();
        //vars
        int ssocket;
        struct sockaddr_in cdata_socket;
        struct timeval clientTime;
        int state;
        //added by sel-jama
        int r_done;
        int w_done;
};



#endif