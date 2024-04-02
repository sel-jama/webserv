#include "server/Socket.hpp"

int main(int ac, char **av){
    (void)av;
    (void)ac;
    // if (ac != 2)
    //     exit(1);
    
    Socket s;
    // s.clientConnect(av);
    s.serverConnect();
    return 0;
}
