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