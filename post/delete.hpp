#ifndef DELETE_HPP
#define DELETE_HPP

#include <iostream>
#include "../Request/Request.hpp"
#include <map>
// #include "../Cgi/handleCgi.hpp"
#include "../Response/method.hpp"

class handleCgi;
class Delete{

    private :
    public : 
        std::string Path;
        int variable;
        std::string Type;
        void check_RequestedR(Request &obj);
        void Work_with_file(void);
        void Work_with_Directory();
        void R_removing();
};

class Except : std::exception{
    const char * what() const throw(){
        return ("Error : Not found 404");
    }
};
#endif