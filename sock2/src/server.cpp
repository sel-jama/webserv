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
		}
		else throw(std::runtime_error("Error : config-file :bad config file in location not a key"));
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
	if (pages.size() <= 1)throw(std::runtime_error("Error : config-file :bad config file\" Error-pages :check number of arguments in  error-pages\""));
	if (*it != ";") throw(std::runtime_error("Error : config-file :bad config file\" Error-pages :add \";\"at the end of the path of error-pages\""));
	++it;
	std::string path;
	std::vector<std::string>::const_iterator it2 = pages.end() - 1;
	int state = 0;
	if ((*it2).at(0) == '/' && ((*it2).at(1) == '~' || !(*it2).empty())) path = *it2;
	for (std::vector<std::string>::iterator it3 = pages.begin(); it3 != pages.end(); ++it3)
	{
		for (std::map<std::string, std::string>::iterator ito = errorPages.begin(); ito != errorPages.end() ; ++ito)
		{
			if ((*it3) == (ito)->first)
			{
				errorPages[*it3] = path;
				state = 1;
				break;
			}
		}
		if (state == 0) throw(std::runtime_error("Error : config-file :bad config file\" Error-pages : give valide error-pages please\""));
	}
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

void server::setServerName(std::vector<std::string>::const_iterator &it, std::vector<std::string> &infra_names, int &i, const std::vector<std::string> &tokens)
{
	++it;
	++i;
	if (i != 1) throw(std::runtime_error("Error : config-file :bad config file\" server-name :only one server-name per server\""));
	while (*it != ";" && it != tokens.end())
	{
		if ((*it).empty())throw(std::runtime_error("Error : config-file :bad config file\" server-name: empty server name\""));
		for (std::vector<std::string>::const_iterator it2 = infra_names.begin(); it2 != infra_names.end(); ++it2)
			if ((*it2) == (*it))throw(std::runtime_error("Error : config-file :bad config file\" server-name :can't have 2 same server-name in different servers\""));
		//check if server name khssou itekteb bchi tari9a mou3ayana
		/////////////
		////////////
		infra_names.push_back(*it);
		serverName.push_back(*it);
		++it;
	}
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
		port = htons(i);
	}
	else throw(std::runtime_error("Error : config-file :bad config file\" listen => correct way to write listen \n @:port please :)\""));
	++it;
}


server::server()
{
	//error-page number
	errorPages["300"] = "";
	errorPages["301"] = "";
	errorPages["400"] = "";
	errorPages["403"] = "";
	errorPages["404"] = "";
	errorPages["405"] = "";
	errorPages["408"] = "";
	errorPages["409"] = "";
	errorPages["411"] = "";
	errorPages["413"] = "";
	errorPages["500"] = "";
	errorPages["501"] = "";
	errorPages["504"] = "";
	errorPages["505"] = "";
	//client max body size
	clientMaxBodySize = 2000000000000000000;

}





server::~server(){}

// server::server(const server &ser):()
// {}

const u_int16_t & server::getPort()const{return(port);}
const std::string & server::getAdress()const{return(adress);}
const long long & server::getClientMaxBodySize()const{return(clientMaxBodySize);}
const std::vector<std::string> & server::getServerName()const{return(serverName);}

//added by sel-jama
const std::vector<location> &server::getLocations()const{return locations;}


void server::checkServerData()
{
	//listen manda --> check what is manda and what is not
	if (port == 0) throw(std::runtime_error("Error : config-file :bad config file\" empty port\""));
	if (adress.empty()) throw(std::runtime_error("Error : config-file :bad config file\" empty adress\""));
	if (locations.empty()) throw(std::runtime_error("Error : config-file :bad config file\" empty location\""));

}

void server::printServer()
{
	checkServerData();
	std::cout << "Port value : " << port << std::endl;
	std::cout << "Port Adress : " << adress << std::endl;
	for (std::map<std::string, std::string>::iterator it = errorPages.begin(); it != errorPages.end(); ++it)
	{
		if (!it->second.empty())
			std::cout << "Error page: " << it->first << " ==> " << it->second << std::endl;
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
	for(std::vector<client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (time(NULL) - it->wakt > 60)
			clientdown(*it, r, w, maxfd);
	}
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
	//add state that it's not read yet => sel
	if ((tmp.ssocket = accept(ssocket, (struct sockaddr *)&tmp.cdata_socket, &len)) == -1)	throw(std::runtime_error("Error: init clients : accept()"));
	if (fcntl(tmp.ssocket, O_NONBLOCK) == -1)											throw(std::runtime_error("Error: init clients : fcntl()"));
	FD_SET(tmp.ssocket , &fd_r);
	gettimeofday(&tmp.clientTime, NULL);
	//--------------mid-merge
	
	clients.push_back(tmp);
	maxfd = tmp.ssocket;
}

void server::ioswap(fd_set &y, fd_set &no, int fd)
{
	FD_SET(fd, &y);
	FD_CLR(fd, &no);
}

void server::handle_old_cnx(fd_set &fd_r, fd_set &fd_w, fd_set &fd_rcopy, fd_set &fd_wcopy, int &maxfd)
{

	(void)fd_wcopy, (void)fd_rcopy;
	for (std::vector<client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (FD_ISSET((*it).ssocket, &fd_rcopy))
		{
			if (!((*it).reqq).getcheckRequest((*it, *this))) 
			{
				clientdown(*it, fd_r, fd_w, maxfd);
				continue;
			}
			if ((*it).request_done) ioswap(&fd_r, &fd_w, (*it).ssocket);
		}
		else if (FD_ISSET((*it).ssocket, &fd_wcopy) && readisdone)//to recheck
		{
			if (!((*it).reqq).send_response(*it, *this)) 
			{
				clientdown(*it, fd_r, fd_w, maxfd);
				continue;
			}
			if ((*it).respond_done) ioswap(&fd_w, &fd_r, (*it).ssocket);
		}
	}
}