#include "../includes/server.hpp"

void server::setlocation(std::vector<std::string>::const_iterator &it, const std::vector<std::string> &tokens)
{
	int i = 0;
	int j = 0;
	location tmp;
	++it;
	tmp.setuplocationName(*it);
	++it;
	if (*it != "{")throw(std::runtime_error("Error : config-file :bad config file\" location :add \"{\"at the begining of the location\""));
	++it;
	while(*it!= "}" && it != tokens.end())
	{
		if (iskey2(*it))
		{
			if (*it == "upload")                tmp.setUpload(it);
			else if (*it == "autoindex")        tmp.setAutoIndex(it);
			else if (*it == "http_methods")    tmp.setMethods(it, tokens);
			else if (*it == "return")           tmp.setReturn(it, i);
			else if (*it == "root")             tmp.setRoot(it, j);
			else if (*it == "cgi")              tmp.setCgi(it);
			else if (*it == "index")			tmp.setIndex(it);
			else if (*it == "upload_path")		tmp.setUploadPath(it);
		}
		else {
			std::cout << "eror :" << *it << std::endl;
			throw(std::runtime_error("Error : config-file :bad config file in location not a key"));
			}
	}
	if (*it == "}") ++it ;
	else throw(std::runtime_error("Error : config-file :bad config file\" location :add \"}\"at the end of the location\""));
	locations.push_back(tmp);
}

void server::setErrorpages(std::vector<std::string>::const_iterator &it, const std::vector<std::string> &tokens)
{
	++it;
	std::vector<std::string> pages;
	while(*it != ";" && it != tokens.end())
	{
		pages.push_back(*it);
		++it;
	}
	if (pages.size() != 2)throw(std::runtime_error("Error : config-file :bad config file\" Error-pages :check number of arguments in  error-pages\""));
	if (*it != ";") throw(std::runtime_error("Error : config-file :bad config file\" Error-pages :add \";\"at the end of the path of error-pages\""));
	++it;
	errorPages[*(pages.begin())] = *(pages.end() - 1) ;
}

void server::setClientMaxBodySize(std::vector<std::string>::const_iterator &it, int &j)
{
	++it;
	++j;
	if (j != 1)throw(std::runtime_error("Error : config-file :bad config file\" ClientMaxBodySize :only one ClientMaxBodySize per server\""));
	if (!alldigit(*it)) throw(std::runtime_error("Error : config-file :bad config file\" ClientMaxBodySize : only numbers pls\""));
	//check hna ila kaynin chi norms
	clientMaxBodySize = std::stoi(*it);
	++it;
	if (*it != ";") throw(std::runtime_error("Error : config-file :bad config file\" ClientMaxBodySize :add \";\"after ClientMaxBodySize\""));
	++it;
}

void server::setServerName(std::vector<std::string>::const_iterator &it, int &i)
{
	++it;
	++i;
	if (i != 1) throw(std::runtime_error("Error : config-file :bad config file\" server-name :only one server-name per server\""));
	serverNName = *it;
	++it;
	if (*it != ";") throw(std::runtime_error("Error : config-file :bad config file\" server-name :add \";\"after the server names\""));
	++it;
}

void server::setlisten(std::vector<std::string>::const_iterator &it)
{
	std::vector<std::string> tokens;
	// adrrtokens;
	std::string token;
	//  taken;
	++it;
	std::string tmp = *it;
	++it;
	if (*it != ";") throw(std::runtime_error("Error : config-file :bad config file\" listen\""));
	if (!adress.empty())  throw(std::runtime_error("Error : config-file :bad config file\" 2 listen\""));
	if (containalpha(*it)) throw(std::runtime_error("Error : config-file :bad config file\" listen => have an alpha\""));
	std::istringstream iss(tmp);
	if ((tmp).find(':') != std::string::npos)
	{
		while (std::getline(iss, token, ':')) tokens.push_back(token);
		if (tokens.size() != 2) throw(std::runtime_error("Error : config-file :bad config file\" listen => correct way to write listen \n port:@ please :)\""));
		if (tokens.at(1).size() > 6) throw(std::runtime_error("Error : config-file :bad config file\" listen => adjust ur port number Max : 65535\""));
		//hadi anrje3 liha mnin nchuf lblan dyal adress , wach khassha tkoun specefik =>127.0.0.1 ou lmachine
		adress = tokens.at(0);
		if (!alldigit(tokens.at(1))) throw(std::runtime_error("Error : config-file :bad config file\" listen => adjust ur port number Max : 65535\""));
		int i = std::stoi(tokens.at(1));
		if (i < 0 || i > 65535) throw(std::runtime_error("Error : config-file :bad config file\" listen => adjust ur port number Max : 65535\""));
		port = i;
	}
	else throw(std::runtime_error("Error : config-file :bad config file\" listen => correct way to write listen \n @:port please :)\""));
	++it;
}


server::server()
{}



server::~server(){}

const u_int16_t & server::getPort()const{return(port);}
const std::string & server::getAdress()const{return(adress);}
const long long & server::getClientMaxBodySize()const{return(clientMaxBodySize);}
const std::string & server::getServerNName()const{return(serverNName);}
const std::vector<location> &server::getLocations()const{return locations;}


void server::checkServerData()
{
	if (port == 0) throw(std::runtime_error("Error : config-file :bad config file\" empty port\""));
	if (adress.empty()) throw(std::runtime_error("Error : config-file :bad config file\" empty adress\""));
	if (serverNName.empty())throw(std::runtime_error("Error : config-file :bad config file\" empty server_name\""));
	if (!clientMaxBodySize)throw(std::runtime_error("Error : config-file :bad config file\" empty client max body size\""));
	if (adress.empty()) throw(std::runtime_error("Error : config-file :bad config file\" empty location\""));
	if (locations.empty()) throw(std::runtime_error("Error : config-file :bad config file\" empty location\""));
	for (std::vector<location>::iterator it = locations.begin(); it != locations.end(); ++it){(*it).checklocation();}
}

void server::printServer()
{
	checkServerData();
	std::cout << "Port value : " << port << std::endl;
	std::cout << "Port Adress : " << adress << std::endl;

	for (std::map<std::string, std::string>::iterator it = errorPages.begin(); it != errorPages.end(); ++it)
	{
		if (!it->second.empty())
			std::cout << "Error page: " << it->first
		 << " ==> " << it->second 
		 << std::endl;
	}
	std::cout << "client max body size: " << clientMaxBodySize << std::endl;
	for (std::vector<location>::iterator it = locations.begin(); it != locations.end(); ++it)
	{
		it->printlocation();
	}

}


//------------------------------------------------------------------------------------------------------------------------------------------------------


void server::clientdown(client &bclient, fd_set &r, fd_set &w,int &maxfd)
{
	FD_CLR(bclient.ssocket, &r);
	FD_CLR(bclient.ssocket, &w);
	close(bclient.ssocket);
	for (std::vector<client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it).ssocket == bclient.ssocket)
		{
			clients.erase(it);
			break;
		}
	}
	--maxfd;
}

void server::checktime(fd_set &r, fd_set &w, int &maxfd)
{
	size_t j = clients.size(); 
	for(std::vector<client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (time(NULL) - it->wakt > 10){
				clientdown(*it, r, w, maxfd);
				break;}
				
	}
	if (j != clients.size())
		checktime(r, w, maxfd);
}

void server::accept_new_connection(fd_set &fd_r, int &maxfd)
{
	if (clients.size() >= 1022)
	{
		std::cerr << "Max cleint reached , wait for some to close" << std::endl;
		return;
	}
	client tmp;
	memset(&tmp.cdata_socket, 0, sizeof(tmp.cdata_socket));
	socklen_t len = sizeof(tmp.cdata_socket);
	if ((tmp.ssocket = accept(ssocket, (struct sockaddr *)&tmp.cdata_socket, &len)) == -1)	throw(std::runtime_error("Error: init clients : accept()"));
	if (fcntl(tmp.ssocket, O_NONBLOCK) == -1)											throw(std::runtime_error("Error: init clients : fcntl()"));
	FD_SET(tmp.ssocket , &fd_r);
	gettimeofday(&tmp.clientTime, NULL);
	clients.push_back(tmp);
	maxfd = tmp.ssocket;
}

void server::ioswap(fd_set &toadd, fd_set &toremove, int fd)
{
	// std::cout << "hani hna" << std::endl;
	FD_SET(fd, &toadd);
	FD_CLR(fd, &toremove);
}

void server::handle_old_cnx(fd_set &fd_r, fd_set &fd_w, fd_set &fd_rcopy, fd_set &fd_wcopy, int &maxfd, int &k, infra &infra)
{
	size_t j = clients.size();
	for (std::vector<client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (FD_ISSET((*it).ssocket, &fd_rcopy))
		{
			if (!((*it).reqq).read_request(*it, infra))
			{
				clientdown(*it, fd_r, fd_w, maxfd);
				break;
			}
			if ((*it).r_done) ioswap(fd_w, fd_r, (*it).ssocket);
		}
		else if (FD_ISSET((*it).ssocket, &fd_wcopy) && (*it).r_done)//to recheck
		{
			if (!((*it).reqq).send_response(*it)) 
			{
				clientdown(*it, fd_r, fd_w, maxfd);
				break;
			}
			if ((*it).w_done)
			{
				//ioswap(fd_r, fd_w, (*it).ssocket);
				clientdown(*it, fd_r, fd_w, maxfd);
				break;
			}
	}}
	if (j != clients.size())
		handle_old_cnx(fd_r, fd_w, fd_rcopy, fd_wcopy, maxfd, k, infra);
}

server::server(const server &other):
    port(other.port),
    adress(other.adress),
    // serverName(other.serverName),
	serverNName(other.serverNName),
    clientMaxBodySize(other.clientMaxBodySize),
    errorPages(other.errorPages),
    locations(other.locations),
    clients(other.clients),
    ssocket(other.ssocket),
    data_socket(other.data_socket)
{}
server& server::operator=(const server &other)
{
    if (this != &other) // Check for self-assignment
    {
        this->port = other.port;
        this->adress = other.adress;
        // this->serverName = other.serverName;
        this->clientMaxBodySize = other.clientMaxBodySize;
        this->errorPages = other.errorPages;
		this->serverNName = other.serverNName;
        this->locations = other.locations;
        this->clients = other.clients;
        this->ssocket = other.ssocket;
        this->data_socket = other.data_socket;
    }
    return *this;
}