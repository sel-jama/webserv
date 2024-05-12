#include "../includes/location.hpp"

location::location()
{
    http_methods["POST"]    = 0;
    http_methods["GET"]     = 0;
    http_methods["DELETE"]  = 0;
    autoindex = "on";
    upload = "off";
    HM = 0;
    UP = 0; 
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
    location_name = name;
}

void location::setUpload(std::vector<std::string>::const_iterator &it)
{
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
    ++HM;
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
    std::string tmp;
    while (*it != ";")
    {
        tmp += (*it);
        ++it;
        if (*it != ";")
            tmp += ' '; 
    }
    if (*it != ";")throw(std::runtime_error("Error : config-file :bad config file\" root : finish with ;"));
        ++it;
    retu = tmp;
}

void location::setRoot(std::vector<std::string>::const_iterator &it, int &i)
{
    if (i == 1) throw(std::runtime_error("Error : config-file :bad config file\" root : One root per location"));
    ++i;
    ++it;
    root = *it;
    ++it;
    if (*it != ";")throw(std::runtime_error("Error : config-file :bad config file\" root : finish with ;"));
    ++it;
}

void location::setCgi(std::vector<std::string>::const_iterator &it)
{
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
    if (root.empty()) throw(std::runtime_error("Error: config-file : root => need root in location"));
    if (location_name.empty()) throw(std::runtime_error("Error: config-file : location name => need location_name in location"));
    if (upload.empty())throw(std::runtime_error("Error: config-file : upload => need upload in location"));
    if (autoindex != "on" && autoindex != "off") throw(std::runtime_error("Error: config-file : autoindex => need autoindex in location"));
    if (!HM)throw(std::runtime_error("Error: config-file : Http_methodes => need http_methods in location"));
    if (root.find("/../") != std::string::npos) throw(std::runtime_error("pb in root path"));
    if (root.find("/..") != std::string::npos) throw(std::runtime_error("pb in root path"));
    if (root.find("../") != std::string::npos) throw(std::runtime_error("pb in root path"));
}



void location::setIndex(std::vector<std::string>::const_iterator &it)
{
    ++it;
    while ((*it) != ";")
    {
        index.push_back(*it);
        ++it;
    }
    if (*it != ";")throw(std::runtime_error("Error : config-file :bad config file\" root : finish with ;"));
    ++it;
}

void location::setUploadPath(std::vector<std::string>::const_iterator &it)
{
    ++it;
    std::string tmp;
    while (*it != ";")
    {
        tmp += (*it);
        ++it;
        if (*it != ";")
            tmp += ' '; 
    }
    if (*it != ";")throw(std::runtime_error("Error : config-file :bad config file\" root : finish with ;"));
        ++it;
    upload_path = tmp;
}

void location::printlocation()
{
    checklocation();
    std::cout << "location name : " << location_name << std::endl;
    if (!upload.empty())
        std::cout << "        =>" << "upload    : " << upload << std::endl;
    if (!upload_path.empty())
        std::cout << "upload path   : " << upload_path << std::endl;
    std::cout << "        =>" << "autoindex : " << autoindex << std::endl;
    std::cout << "        =>" << "https_methodes : " << std::endl;
    std::cout << "           Get    : "  << http_methods["GET"] << std::endl;
    std::cout << "           Post   : "  << http_methods["POST"] << std::endl;
    std::cout << "           Delete : "  << http_methods["DELETE"] << std::endl;
    if (!retu.empty())
    {
        std::cout << "        =>" << " returns  : "  << retu << std::endl;

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
    if (!index.empty())
    {
        std::cout << "        =>" << " index : " << std::endl;
        for(std::vector<std::string>::iterator it = index.begin(); it != index.end(); ++it)
        {
            std::cout << "          index : " << (*it) << std::endl;
        }
    }
}
