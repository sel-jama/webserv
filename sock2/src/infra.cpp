#include "../includes/infra.hpp"
#include <sys/socket.h>
#include "../../Response/Response.hpp"

//---------init servers -> socket->fcntrl->setsocketopt->bind->listen->select->accept

void infra::sockettolisten(std::vector<server>::iterator &it)
{
    // check_port();
    int opt = 1;
    if (((*it).ssocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)                        throw(std::runtime_error("Error: init servers : socket()"));
    if (fcntl((*it).ssocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)                     throw(std::runtime_error("Error: init servers : fcntl("));
    if (setsockopt((*it).ssocket, SOL_SOCKET, SO_REUSEPORT, &(opt), sizeof(opt)) == -1) throw(std::runtime_error("Error: init servers : setsockopt() SO_REUSPORT"));
    if (bind((*it).ssocket, (*it).servinfo->ai_addr, (*it).servinfo->ai_addrlen) == -1)           throw(std::runtime_error("Error: init servers : bind()"));
    if (listen((*it).ssocket, SOMAXCONN ) == -1)                                                throw(std::runtime_error("Error: init servers : listen()"));
}

void infra::initselect()
{
    maxfd = -1;
    FD_ZERO(&fd_r);
    FD_ZERO(&fd_w);
    for (std::vector<server>::iterator it = servers.begin(); it != servers.end(); ++it)
    {
        FD_SET((*it).ssocket, &fd_r);
        if ((*it).ssocket > maxfd) maxfd = (*it).ssocket;
    }
    timeout.tv_sec = 15;
    timeout.tv_usec = 0;
}



std::string int_to_string(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

void infra::initdata(std::vector<server>::iterator &it)//to be modifed
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    std::string ip2 = (*it).getAdress();
    std::string port2 = int_to_string((*it).getPort());
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(ip2.c_str() , port2.c_str(), &(hints), &((*it).servinfo)) != 0) throw(std::runtime_error("pb"));
}


void infra::selecttoinfinity()
{
    signal(SIGPIPE, SIG_IGN);
    while (1)
    {
        fd_rcopy = fd_r;
        fd_wcopy = fd_w;
        int slct = select(maxfd + 1, &fd_rcopy, &fd_wcopy, NULL, &timeout);
        if (slct == -1) throw(std::runtime_error("Error : select : lanch"));
        if (slct == 0)
        {
            timeout.tv_sec = 10;
            timeout.tv_usec = 0;
            for (std::vector<server>::iterator it = servers.begin(); it != servers.end(); ++it)
                (*it).checktime(fd_r, fd_w, maxfd);
            continue;
        }
        for (std::vector<server>::iterator it = servers.begin(); it !=servers.end(); ++it)
        {
            if (FD_ISSET((*it).ssocket, &fd_rcopy))
            {
                (*it).accept_new_connection(fd_r, maxfd, (*it));
            }
            else 
                (*it).handle_old_cnx(fd_r, fd_w, fd_rcopy, fd_wcopy, maxfd, *this, 0);
        }

    }
}

void infra::initservers()
{
    for (std::vector<server>::iterator it = servers.begin(); it != servers.end(); ++it)
    {
        initdata(it);
        sockettolisten(it);
    }
    initselect();
    selecttoinfinity();
}


//----------parse confid file-------------------------------------------//
infra::infra(const std::vector<std::string> &tokens)
{
    std::vector<std::string>::const_iterator it = tokens.begin();
    while (it != tokens.end())
    {
        if (*it == "server")
        {
            server serverdata;
            ++it;
            if (*it == "{")
            {
                ++it;
                int i = 0;//to check if there is one servername
                int j = 0;// one mbs
                while(*it != "}" && it != tokens.end())
                {
                    if (iskey(*it))
                    {
                        if (*it == "listen") serverdata.setlisten(it);
                        if (*it == "server_name") serverdata.setServerName(it, server_name_list, i, tokens);
                        if (*it == "client_max_body_size") serverdata.setClientMaxBodySize(it, j);
                        if (*it == "error_page") serverdata.setErrorpages(it, tokens);
                        if (*it == "location") serverdata.setlocation(it, tokens);
                    }
                    else throw(std::runtime_error("Error : config-file :bad config file not a key"));
                }
                if (*it != "}")throw(std::runtime_error("Error : config-file :missed a }"));
                    ++it;
            servers.push_back(serverdata);
            }
            else throw(std::runtime_error("Error : config-file :bad config file {"));
        }
        else throw(std::runtime_error("Error : config-file :bad config file server"));
    }
    checkInfraData();
    // printInfra();
}


//----------------debuging------------------------------------//
infra::~infra(){}

void infra::checkInfraData()
{
    if (servers.empty()) throw(std::runtime_error("Error : config-file : ss ur servers !!"));
    for (std::vector<server>::iterator it = servers.begin(); it != servers.end(); ++it){(*it).checkServerData();}
    for (std::vector<server>::iterator it = servers.begin(); it != servers.end() - 1; ++it)
    {
        for (std::vector<server>::iterator it2 = it + 1; it2 != servers.end(); ++it2)
        {
            // if ((*it).port == (*it2).port) 
            // {
            //     std::cout << "port 1 :" << (*it).port << "\n prt 2 :"   << (*it2).port <<std::endl;
            //     throw(std::runtime_error("Duplicate port numbers"));
            //     }
        }
    }

}

void infra::printInfra()
{
    checkInfraData();
    int i = 1;
    for(std::vector<server>::iterator it = servers.begin(); it != servers.end(); ++it)
    {
        std::cout << "server :" << i << std::endl;
        (it)->printServer();
        ++i;
        std::cout << "============================================" << std::endl;
    }
}