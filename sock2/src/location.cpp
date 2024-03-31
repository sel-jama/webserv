#include "../includes/location.hpp"

location::location()
{
    http_methods["POST"]    = 0;
    http_methods["GET"]     = 0;
    http_methods["DELETE"]  = 0;
    autoindex = "on";
}

location::~location(){}

std::string location::location_n()
{
    return location_name;}

std::string location::upl()
{
    return(upload);}

std::map <std::string, int> location::http_m(){
    return(http_methods);}

std::map<std::string, std::string> location::cg(){return cgi;};


void location::setuplocationName(const std::string &name)
{
    //check path ?
    location_name = name;
}

void location::setUpload(std::vector<std::string>::const_iterator &it)
{
    //check path?
    ++it;
    upload = *it;
    ++it;
    if (*it != ";") throw(std::runtime_error("Error : config-file :bad config file\" upload :add \";\"at the end of the path of error-pages\""));
    ++it;
}

void location::setAutoIndex(std::vector<std::string>::const_iterator &it)
{
    ++it;
    if (*it == "on" || *it == "off") autoindex = *it;
    else throw(std::runtime_error("Error : config-file :bad config file\" autoindex bad index"));
    ++it;
    if (*it != ";") throw(std::runtime_error("Error : config-file :bad config file\" autoindex :add \";\"at the end of the path of error-pages\""));
    ++it;
}

void location::setMethods(std::vector<std::string>::const_iterator &it, const std::vector<std::string> &tokens)
{    
    ++it;
    while (*it != ";" && it != tokens.end())
    {
        if (http_methods.find(*it) != http_methods.end())
        {
            http_methods[*it] = 1;
        }
        else throw(std::runtime_error("Error : config-file :bad config file\" autoindex : POST GET OR FELETE AND NOT TWICE"));
        ++it;
    }
    ++it;
}

void location::setReturn(std::vector<std::string>::const_iterator &it, int &i)
{
    if (i == 1) throw(std::runtime_error("Error : config-file :bad config file\" return : One return per location"));
    ++i;
    ++it;
    //do i hcekc the path ?
    std::string tmp;
    tmp = *it;
    ++it;
    returns[std::stoi(tmp)] = *it;  
    ++it;
    if (*it != ";")throw(std::runtime_error("Error : config-file :bad config file\" return : finish with ;"));
    ++it;
}

void location::setRoot(std::vector<std::string>::const_iterator &it, int &i)
{
    if (i == 1) throw(std::runtime_error("Error : config-file :bad config file\" root : One root per location"));
    ++i;
    ++it;
    //do i check root ?
    root = *it;
    ++it;
    if (*it != ";")throw(std::runtime_error("Error : config-file :bad config file\" root : finish with ;"));
    ++it;
}

void location::setCgi(std::vector<std::string>::const_iterator &it)
{
    //check cgi type ?
    std::string tmp;
    ++it;
    tmp = *it;
    ++it;
    cgi[tmp] = *it;
    ++it;
    if (*it != ";")throw(std::runtime_error("Error : config-file :bad config file\" root : finish with ;"));
    ++it;
}

void location::checklocation()
{
    if (root.empty()) throw(std::runtime_error("Error: config-file : root => n eed root in location"));
}

void location::printlocation()
{
    checklocation();
    std::cout << "location name : " << location_name << std::endl;
    if (!upload.empty())
        std::cout << "        =>" << "upload    : " << upload << std::endl;
    std::cout << "        =>" << "autoindex : " << autoindex << std::endl;
    std::cout << "        =>" << "https_methodes : " << " Get : " ;
    if (http_methods["GET"] == 1) std::cout << "yes";
    else std::cout << "no " ;
    std::cout << ": Post : " ;
    if (http_methods["POST"] == 1) std::cout << "yes";
    else std::cout << "no " ;
    std::cout << ": Delete : " ;
    if (http_methods["DELETE"] == 1) std::cout << "yes";
    else std::cout << "no " ;
    std::cout << std::endl;
    if (!returns.empty())
    {
        std::cout << "        =>" << " returns  : " << std::endl;
        for (std::map <int , std::string>::iterator j = returns.begin(); j != returns.end(); ++j)
        {
            std::cout << "        num : " << j->first << " path " << j->second <<std::endl; 
        }
    }
    std::cout << "        => root     : " << root << std::endl;
    if (!cgi.empty())
    {
        std::cout << "        =>" << " cgi : " << std::endl;
        for (std::map <std::string , std::string>::iterator j = cgi.begin(); j != cgi.end(); ++j)
        {
            std::cout << "          type : " << j->first << " path " << j->second <<std::endl; 
        }
    }
}
