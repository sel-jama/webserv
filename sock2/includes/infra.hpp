#ifndef INFRASTRUCTOR_HPP
#define INFRASTRUCTOR_HPP

#include "server.hpp"
#include "client.hpp"
#include "WebServer.hpp"
#include "utils.hpp"
#include <signal.h>


//parse the config file to servers


class infra
{
    private:
        std::vector<server> servers;
        std::vector<client> clients;
        std::vector<std::string> server_name_list;
        std::vector<int> socket_lstn;
        infra(){}
        infra(const infra &);
        const infra &operator=(const infra &);

    public:
        //setter 
        //getter
        fd_set fd_r, fd_rcopy, fd_w, fd_wcopy;
        int maxfd;
        struct timeval timeout;
        
        const std::vector<server> &getServer()const{return servers;}
        const std::vector<client> &getClients()const{return clients;}
        
        //canonical form
        ~infra();

        //work with servers
        void initservers();
        void initdata(std::vector<server>::iterator &);
        void sockettolisten(std::vector<server>::iterator &);
        void initselect();
        void selecttoinfinity();

        //accept or else
        //void accept_new_connection(int i);
        //void handle_old_cnx(int i);

        //check - debug
        void printInfra();
        void checkInfraData();

        //constructr valid
        infra(const std::vector<std::string> &);
        
        int check_port();


};

#endif