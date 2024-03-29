#include "../includes/infra.hpp"
#include <sys/socket.h>

//---------init servers -> socket->fcntrl->setsocketopt->bind->listen->select->accept

void infra::sockettolisten(std::vector<server>::iterator &it)
{
    int opt = 1;
    socklen_t j = sizeof((*it).data_socket);
    if (((*it).ssocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)                        throw(std::runtime_error("Error: init servers : socket()"));
    if (fcntl((*it).ssocket, O_NONBLOCK) == -1)                                         throw(std::runtime_error("Error: init servers : fcntl()"));
    if (setsockopt((*it).ssocket, SOL_SOCKET, SO_REUSEPORT, &(opt), sizeof(opt)) == -1) throw(std::runtime_error("Error: init servers : setsockopt() SO_REUSPORT"));
    if (bind((*it).ssocket, (const sockaddr *)&((*it).data_socket), j) == -1)           throw(std::runtime_error("Error: init servers : bind()"));
    if (listen((*it).ssocket, 10) == -1)                                                throw(std::runtime_error("Error: init servers : listen()"));

}


void infra::initselect()
{
    maxfd = -1;
    FD_ZERO(&fd_r);
    FD_ZERO(&fd_w);
    for (std::vector<int>::iterator it = socket_lstn.begin(); it != socket_lstn.end(); ++it)
    {
        FD_SET(*it, &fd_r);
        if (*it > maxfd) maxfd = *it;
    }
    timeout.tv_sec = 5;
}


void infra::initdata(std::vector<server>::iterator &it)//to be modifed
{
    (*it).data_socket.sin_addr.s_addr = INADDR_ANY;
    (*it).data_socket.sin_port = (*it).getPort();
    (*it).data_socket.sin_family = AF_INET;

}

void infra::accept_new_connection(int fd)
{
    //check if hit max fd
    client tmp;
    memset(&tmp.cdata_socket, 0, sizeof(tmp.cdata_socket));
    socklen_t len = sizeof(tmp.cdata_socket);
    tmp.state = 0;
    if ((tmp.ssocket = accept(fd, (struct sockaddr *)&tmp.cdata_socket, &len)) == -1) 
        throw(std::runtime_error("Error: init clients : accept()"));
    if (fcntl(tmp.ssocket, O_NONBLOCK) == -1)      throw(std::runtime_error("Error: init clients : fcntl()"));                                                   throw(std::runtime_error("Error: init clients : fcntl()"));
    FD_SET(tmp.ssocket , &fd_r);
    gettimeofday(&tmp.clientTime, NULL);
    clients.push_back(tmp);
    maxfd = tmp.ssocket;
}

// void infra::handle_old_cnx(int i)
// {
//     //2cases : request or respond
//     if (FD_ISSET(i, fd_r))
//     {
//         if (read_done(i)) // handle request
//         {

//             continue;
//         }
//         if (state == 1) // ssala read
//         {

//         }
//     }
//     else if (FD_ISSET(i, fd_w))
//     {
//         if (write_done(i)) // handle respond
//         {

//         }
//         if (state == 0) // ssala write
//         {
            
//         }
//     }
// }

void infra::selecttoinfinity()
{
    // int slct;
    while (1)
    {
        fd_rcopy = fd_r;
        fd_wcopy = fd_w;
        std::cout << "Waiting for connection... " << std::endl; 
        int slct = select(maxfd + 1, &fd_rcopy, &fd_wcopy, NULL, &timeout);
        if (slct == -1) throw(std::runtime_error("Error : select : lanch"));
        if (slct == 0)  
        {
            //close sleeping clients // no data in the specefic timeout
            throw(std::runtime_error("Error : select : timeout"));
        }
        // for (std::vector<server>::iterator it = servers.begin(); it !=servers.end(); ++it)
        // {
        //     if (FD_ISSET((*it).ssocket, &fd_r))
        //     {
        //         accept_new_connection(i);
        //         continue;
        //     }
        //     else
        //         handle_connection(i);
        // }

    }
}

void infra::initservers()
{
    for (std::vector<server>::iterator it = servers.begin(); it != servers.end(); ++it)
    {
        initdata(it);
        sockettolisten(it);
        socket_lstn.push_back(it->ssocket);
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
    // printInfra();
}


//----------------debuging------------------------------------//
infra::~infra(){}

void infra::checkInfraData()
{
    if (servers.empty()) throw(std::runtime_error("Error : config-file : ss ur servers !!"));
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