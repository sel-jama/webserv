#ifndef POST_HPP
#define POST_HPP

#include <exception>
#include <fstream>
#include <iostream>
// #include <strstream>
#include <map>
#include "../Request/Request.hpp"
#include "../cgi/handleCgi.hpp"
#include "../sock2/includes/client.hpp"

class Post{
    private:
        // std::map<std::string, std::string> extension;
        bool post_indicate;
        bool chunked_indicate;
        std::string string_saver;
        std::string path;
        size_t reading_count;
        // int count;
        size_t content_lenght;
    public:
        Post(){};
        int saver_count;
        int finish;
        std::string Type;
        int i;
        size_t to_de;
        std::string Body;
        int flag;
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
        void support_upload(Request &obj);
        void get_Request_resource(Request obj);
        void After_geting_resource(Request obj);
        void Work_with_file(Request obj);
        void Work_with_Directory(Request obj);
        static void body(client &obj);
        static void chunked_body(client &obj);
        static void chunked_body2(client &obj);
        ~Post(){};
};
int hexa_to_num(std::string ptr);
class Except : std::exception{
    const char * what() const throw(){
        return ("Error : Not found 404");
    }
};

#endif

