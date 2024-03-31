#ifndef POST_HPP
#define POST_HPP

#include <iostream>
#include <strstream>
#include <sys/_types/_size_t.h>
#include <map>

class Post{
    private:
        std::map<std::string, std::string> extension;
        bool post_indicate;
        bool chunked_indicate;
        std::string string_saver;
        std::string path;
        size_t reading_count;
        size_t content_lenght;
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
    Post();
    ~Post();
};

int hexa_to_num(std::string ptr);


#endif

