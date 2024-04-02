#include "post.hpp"
#include "../Request/Request.hpp"
#include <sstream>
#include <string.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>

bool Post::getpost_i(){
    return post_indicate;
}

bool Post::getchunked_i(){
    return chunked_indicate;
}

std::string Post::get_saver(){
    return string_saver;
}

void Post::set_saver(std::string value)
{
    this->string_saver = value;
}

void Post::set_post_i(bool value){
    this->post_indicate = value;
}

void Post::set_chunked_i(bool value){
    this->chunked_indicate = value;
}

std::string Post::get_path(){
    return path;
}

void Post::set_path(std::string value){
    this->path = value;
}

void Post::set_count(size_t value){
    this->reading_count = value;
}

size_t Post::get_last_read(){
    return reading_count;
}

size_t Post::get_content_lenght(){
    return content_lenght;
}

void Post::set_content_lenght(size_t value){
    this->content_lenght = value;
}

int hexa_to_num(std::string ptr)
{
    int value;
    std::stringstream str(ptr);
    str >> std::hex >> value;
    return (value);
}

void Post::load_extension()
{
    std::ifstream file("MIME.conf");
    std::string buffer;
    std::string secbuffer;
    std::string forvalue;
    std::string forkey;
    if (!file.is_open()) {
        while(std::getline(file, buffer))
        {
                std::stringstream ss(buffer);
                for (int i = 0; getline(ss, secbuffer, ' '); i++) {
                    if (i == 0)
                        forkey = secbuffer;
                    if(i == 1)
                    {
                            extension[forkey] = secbuffer;
                            break;
                    }
                }
        }
    }
}

void Post::load_data(int file)
{
    //if(!file.is_open())
    //{
        //moving data normally with content_lenght;
    //}
}

void Post::support_upload(Request obj){
    int check = 0;
    location capt = obj.getMatchedLocation();
    std::string  ptr = capt.upl();
    if(ptr.empty())
        throw Except();
    else
    {
        const char* ptr= obj.path.c_str();  
        check = access(ptr, F_OK);
        if (check == -1) {
            throw Except();
        }
        else {
           std::ofstream file(obj.fileName);
           if (!file.is_open()) {
               load_data(check); 
           }
        }
    }
        
}
