#include "../includes/client.hpp"

// client::client(int fd, fd_set &fd_r, int &maxfd)
client::client()
{
    wakt = time(NULL);
    //handle other vars
    r_done = 0;
    w_done = 0;
    
}
client::~client(){}

void client::reset_client()
{
    wakt = time(NULL);
    //handle other vars
    r_done = 0;
    w_done = 0;
}

client::client(const client &other) :
    ssocket(other.ssocket),
    cdata_socket(other.cdata_socket),
    clientTime(other.clientTime),
    r_done(other.r_done),
    w_done(other.w_done),
    wakt(other.wakt),
    port_server(other.port_server),
    adress_server(other.adress_server),
    reqq(other.reqq)
{}

  
client& client::operator=(const client &other)
{
    if (this != &other)
    {
        ssocket = other.ssocket;
        cdata_socket = other.cdata_socket;
        clientTime = other.clientTime;
        r_done = other.r_done;
        w_done = other.w_done;
        wakt = other.wakt;
        reqq = other.reqq;
        adress_server = other.adress_server;
        port_server = other.port_server;
    }
    return *this;
}