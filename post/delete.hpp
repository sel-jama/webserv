#ifndef DELETE_HPP
#define DELETE_HPP

#include <iostream>
#include "../Request/Request.hpp"
#include <map>
class Delete{

    private :
    public : 
        std::string Type;
        void check_RequestedR(Request obj);
        void Work_with_file(Request obj);
        void Work_with_Directory(Request obj);
};

class Except : std::exception{
    const char * what() const throw(){
        return ("Error : Not found 404");
    }
};
#endif