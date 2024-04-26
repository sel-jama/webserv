#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "WebServer.hpp"

class location
{
    public:
        std::string location_name;
        std::string upload;
        std::string autoindex;
        std::map <std::string , int> http_methods;
        std::map <int , std::string> returns;
        std::string retu;
        std::string root;
        std::map <std::string, std::string> cgi;
        int HM, UP; //httpmethoes uploafpath
        //---added last
        std::vector<std::string> index;
        std::string upload_path;
        //--------

        void setuplocationName(const std::string &);
        void setUpload(std::vector<std::string>::const_iterator &);
        void setAutoIndex(std::vector<std::string>::const_iterator &);
        void setMethods(std::vector<std::string>::const_iterator &, const std::vector<std::string> &);
        void setReturn(std::vector<std::string>::const_iterator &, int &);
        void setRoot(std::vector<std::string>::const_iterator &, int &);
        void setCgi(std::vector<std::string>::const_iterator &);
        void setIndex(std::vector<std::string>::const_iterator &);
        void setUploadPath(std::vector<std::string>::const_iterator &);


        std::string location_n();
        std::string upl();
        std::map <std::string, int> http_m();
        std::map <std::string, std::string> cg();

        location();
        ~location();

        void printlocation();
        void checklocation();

};

#endif
