#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "WebServer.hpp"
#include "../../Request/Request.hpp"
// class Request;

class client
{
    private:

    public:
        client(const client &);
        client &operator=(const client &);
        // client(int fd, fd_set &fd_r, int &maxfd)
        client();
        ~client();
        //vars
        int ssocket;
        struct sockaddr_in cdata_socket;
        struct timeval clientTime;
        //added by sel-jama
        int r_done;
        int w_done;
        //v1.1
        time_t wakt;

        //mid-merge
        Request reqq;
        void reset_client();    
};



#endif