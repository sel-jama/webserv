#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "WebServer.hpp"
#include "../../Request/Request.hpp"
// class Request;

class client
{
    public:
        client(const client &);
        client &operator=(const client &);
        client();
        ~client();
        //vars
        int ssocket;
        struct sockaddr_in cdata_socket;
        struct timeval clientTime;
        int r_done;
        int w_done;
        time_t wakt;
        uint16_t                            port_server;
        std::string                         adress_server;
        Request reqq;
        void reset_client();    
};
#endif