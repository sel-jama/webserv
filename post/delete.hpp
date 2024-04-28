#ifndef DELETE_HPP
#define DELETE_HPP

#include <iostream>
#include "../Request/Request.hpp"
#include <map>
#include "../Cgi/handleCgi.hpp"
#include "../Response/method.hpp"

class handleCgi;
class Delete{

    private :
    public : 
        std::string Type;
        void check_RequestedR(Request &obj);
        void Work_with_file(Request &obj, handleCgi &o);
        void Work_with_Directory(Request &obj, handleCgi &o);
        void R_removing(Request &obj);
};

class Except : std::exception{
    const char * what() const throw(){
        return ("Error : Not found 404");
    }
};
#endif