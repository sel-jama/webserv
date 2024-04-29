#include "../includes/infra.hpp"
#include <sys/socket.h>
#include "../../Response/Response.hpp"

//---------init servers -> socket->fcntrl->setsocketopt->bind->listen->select->accept

// int infra::check_port()
// {
//     if (port)
// }

void infra::sockettolisten(std::vector<server>::iterator &it)
{
    // check_port();
    int opt = 1;
    socklen_t j = sizeof((*it).data_socket);
    if (((*it).ssocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)                        throw(std::runtime_error("Error: init servers : socket()"));
    if (fcntl((*it).ssocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)                                         throw(std::runtime_error("Error: init servers : fcntl()"));
    if (setsockopt((*it).ssocket, SOL_SOCKET, SO_REUSEPORT, &(opt), sizeof(opt)) == -1) throw(std::runtime_error("Error: init servers : setsockopt() SO_REUSPORT"));
    if (bind((*it).ssocket, (const sockaddr *)&((*it).data_socket), j) == -1)           throw(std::runtime_error("Error: init servers : bind()"));
    if (listen((*it).ssocket,10 ) == -1)                                                throw(std::runtime_error("Error: init servers : listen()"));
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
    timeout.tv_sec = 100;
}


void infra::initdata(std::vector<server>::iterator &it)//to be modifed
{

    (*it).data_socket.sin_addr.s_addr = INADDR_ANY;
    (*it).data_socket.sin_port = htons((*it).getPort());
    (*it).data_socket.sin_family = AF_INET;

}


void infra::selecttoinfinity()
{
    signal(SIGPIPE, SIG_IGN);
    int k = 0;
    while (1)
    {
        fd_rcopy = fd_r;
        fd_wcopy = fd_w;
        // std::cout << "Waiting for connection... " << std::endl;
        int slct = select(maxfd + 1, &fd_rcopy, &fd_wcopy, NULL, &timeout);
        //std::cout << "=============>jit hnaya : " << slct << std::endl;
        if (slct == -1) throw(std::runtime_error("Error : select : lanch"));
        if (slct == 0)
        {
            std::cout << "ja hna" << std::endl;
            for (std::vector<server>::iterator it = servers.begin(); it != servers.end(); ++it)
                (*it).checktime(fd_r, fd_w, maxfd);
            continue;
        }
        for (std::vector<server>::iterator it = servers.begin(); it !=servers.end(); ++it)
        {
            std::cout << "k==>" << k << std::endl;
            ++k;
            if (FD_ISSET((*it).ssocket, &fd_rcopy))
                (*it).accept_new_connection(fd_r, maxfd);
            else
                (*it).handle_old_cnx(fd_r, fd_w, fd_rcopy, fd_wcopy, maxfd, k);
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
    //check wach chi haja na9ssa ou ila chi extra checks
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