#ifndef POST_HPP
#define POST_HPP

#include <exception>
#include <fstream>
#include <iostream>
#include <strstream>
#include <map>
#include "../Request/Request.hpp"

class Post{
    private:
        std::map<std::string, std::string> extension;
        bool post_indicate;
        bool chunked_indicate;
        std::string string_saver;
        std::string path;
        size_t reading_count;
        size_t content_lenght;
    public:
        Post(){};
        std::string Type;
        int fdsock;
        void set_saver(std::string); 
        void set_post_i(bool value);
        void set_count(size_t value);
        void set_chunked_i(bool value);
        void set_path(std::string path);
        std::string get_saver();
        void set_content_lenght(size_t value);
        bool getchunked_i();
        bool getpost_i();
        std::string get_path();
        size_t get_last_read();
        size_t get_content_lenght();
        void load_extension();
        void support_upload(Request obj);
        void get_Request_resource(Request obj);
        void After_geting_resource(Request obj);
        void Work_with_file(Request obj);
        void Work_with_Directory(Request obj);
        void body(Request obj);
        void chunked_body(Request obj);
        ~Post();
};
class Except : std::exception{
    const char * what() const throw(){
        return ("Error : Not found 404");
    }
};

#endif

