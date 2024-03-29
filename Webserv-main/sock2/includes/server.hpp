#ifndef SERVER_HPP
#define SERVER_HPP

//data for each server
#include "WebServer.hpp"
#include "utils.hpp"
#include "location.hpp"

class server
{
    private:
        uint16_t                            port;
        std::string                         adress;
        std::vector<std::string>            serverName;
        long long                           clientMaxBodySize;
        std::map<std::string, std::string>  errorPages;
        std::vector<location> locations;
        //unused
        const server &operator=(const server &);
    
    public:
        //-------------------------------------//
        //server data x socket
        int ssocket;
        struct sockaddr_in data_socket;
        //setter
        // void setPort(const uint16_t &);
        // void setAdress(const std::string &);
        // void setServerName(const std::vector<std::string> &);
        // void setClientMaxBodySize(long long int &);
        
        //getter
        const uint16_t                  &getPort()const;
        const std::string               &getAdress()const;
        const std::vector<std::string>  &getServerName()const;
        const long long                 &getClientMaxBodySize()const;
        const std::vector<location>     &getLocations()const;
        //from configfile -> to server
        void setlisten(std::vector<std::string>::const_iterator &);
        void setServerName(std::vector<std::string>::const_iterator &, std::vector<std::string> &, int &,const std::vector<std::string> &);
        void setClientMaxBodySize(std::vector<std::string>::const_iterator &, int &);
        void setErrorpages(std::vector<std::string>::const_iterator &,const std::vector<std::string> &);
        void setlocation(std::vector<std::string>::const_iterator &, const std::vector<std::string> &);

        //canonical forme
        server();
        ~server();
        // server(const server &);




        //check - debug
        void printServer();
        void checkServerData();

};

#endif