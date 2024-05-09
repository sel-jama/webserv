#include "post.hpp"
#include "../Request/Request.hpp"
#include <string.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "../Response/method.hpp"

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

// void Post::get_Request_resource(Request obj)
// {
//     struct stat buffer;
//     location resource = obj.getMatchedLocation();
//     std::string url = obj.getUri();
//     std::string data = Body;
//     std::string Encoding;
//     std::map<std::string, std::string>::const_iterator value = obj.getHeaders().find("Transfer-Encoding"); 
//     Encoding = value->second;
//     const char *ptr = path.c_str(); 
//     int check = access(ptr, F_OK);
//     if (check  == -1) {
//         throw Except();
//     }
//     // int val = stat(ptr, &buffer);
//     if(S_ISDIR(buffer.st_mode))
//        Type = "Directory";
//     else
//         Type = "File";
// }

void Post::After_geting_resource(Request obj){
    if (Type == "File")
        Work_with_file(obj); 
    else
        Work_with_Directory(obj);
}

void Post::support_upload(Request &obj){
    std::cout <<"body done" << " <<<=========" << std::endl;
    handleCgi obj2;
    int check = 0;
    std::string filename = "";
    obj.load_extension();
    // location obj3 = obj.getMatchedLocation();
    std::map<std::string, std::string>::iterator iter = obj.headers.find("Content-Type");
    // obj.load_extension();
    if(iter != obj.headers.end() && !iter->second.empty())
    {
        obj.content_T = iter->second;
        std::map<std::string, std::string>::iterator iter2 = obj.extension.find(iter->second);
        std::string name = "Post_" + obj2.generateRandomFileName();
        filename = "/"  + name + iter2->second;
    }
        location capt = obj.getMatchedLocation();
        std::string  ptr = capt.upload_path;
        if(ptr.empty())
            throw Except();
    else
    {
        const char* ptr2= ptr.c_str();
        check = access(ptr2, F_OK);
        // std::cout << ptr2 << std::endl;
        if (check == -1) {
            // std::cout << "+++++++++ Im writing " << std::endl;
            throw Except();
        }
        else {
            if(filename.empty())
            {
                std::cout << "HI IM HERE" <<std::endl;
                obj.content_T  = "text/plain";
                filename = "/" + obj2.generateRandomFileName() + ".txt";
            }
                filename = ptr + filename;
                std::ofstream file(filename.c_str());
                obj.cgi_File = filename; 
                obj.cgi_File2 = ptr;
                obj.path = ptr + filename; //
                if (file.is_open() == true)
                {
                    file << obj.body << std::endl;
                    file.close();
                }
                else
                    throw Except();
        }   
    }
}

void Post::Work_with_Directory(Request obj)
{
    
    location capt = obj.getMatchedLocation();
    std::map<std::string, std::string> get = capt.cgi;
    std::map<std::string, std::string>::iterator iter = get.begin();
    std::string search;
    std::string tmp;
    int integ = 0;
    std::vector<std::string>index;
    std::vector<std::string>::iterator for_index = index.begin();

    for(iter = get.begin(); iter != get.end(); iter++)
    {
        search = iter->first;
        search = "." + search;
        if(for_index != index.end())
              tmp = *for_index;
        tmp = tmp.find(search);
        if(!tmp.empty()){
            integ = 1;
            break;
        }
        for_index++;
    }
    if(integ == 1){
        //found it;
        //do cgi work
    }
    else{
       //forbiden 403
        throw Except();
    }
}

void Post::body(client &obj){
    obj.reqq.body.append(obj.reqq.readRequest(obj.ssocket));
    if(static_cast<double>(obj.reqq.body.length()) >= obj.reqq.contentLength){
        obj.reqq.statusCode = 201;
        obj.reqq.responseContentLen = obj.reqq.body.length();
        obj.r_done = 1;
    }
    else
        obj.r_done = 0;
}

std::ofstream Request::file;

void Post::chunked_body(client &obj){
    std::string filename;
    handleCgi obj2;
    location obj3 = obj.reqq.getMatchedLocation();

    std::map<std::string, std::string>::iterator iter = obj.reqq.headers.find("Content-Type");
        if(obj.reqq.flag == 0)
        {
            obj.reqq.load_extension();
             if(iter != obj.reqq.headers.end() && !iter->second.empty())
            {
                obj.reqq.content_T = iter->second;
                std::map<std::string, std::string>::iterator iter2 = obj.reqq.extension.find(iter->second);
                std::string name = "Post_" + obj2.generateRandomFileName();
                filename = "/"  + name + iter2->second;
            }
            else if(filename.empty())
            {
                obj.reqq.content_T = "text/plain";
                filename = "/" + obj2.generateRandomFileName() + ".txt";
            }
            filename = obj3.upload_path + filename;
            obj.reqq.file.open(filename.c_str());
            obj.reqq.cgi_File = filename; 
            obj.reqq.cgi_File2 = obj.reqq.path; 
            obj.reqq.flag = 1;
        }
        obj.reqq.body.append( obj.reqq.readRequest(obj.ssocket));
            if(obj.reqq.saver_count == 0)
            {
                if (obj.reqq.body[0] == '\r' && obj.reqq.body[1] == '\n')
                    obj.reqq.body = obj.reqq.body.substr(2, obj.reqq.body.length());
                obj.reqq.saver_count = obj.reqq.body.find("\r\n");
                obj.reqq.tmp = obj.reqq.saver_count + 2;
                if(obj.reqq.saver_count == 0)
                    throw Except();
                obj.reqq.getit = obj.reqq.body.substr(0, obj.reqq.saver_count);
                obj.reqq.to_de = hexa_to_num(obj.reqq.getit);
                obj.reqq.to_de2 += obj.reqq.to_de;
                obj.reqq.responseContentLen = obj.reqq.to_de2;
                obj.reqq.body = obj.reqq.body.substr(obj.reqq.tmp, obj.reqq.body.size());
            }
            if(obj.reqq.body.size() >= obj.reqq.to_de)
            {
                    obj.reqq.file.write(obj.reqq.body.c_str(), obj.reqq.to_de);
                    obj.reqq.body = obj.reqq.body.substr(obj.reqq.to_de, obj.reqq.body.length());
                    obj.reqq.file.flush();
                    obj.reqq.saver_count = 0;
                }
                if(obj.reqq.body.find("\r\n0\r\n\r\n") != std::string::npos)
                    {
                        std::cout << "im here"<< std::endl;
                        if (obj.reqq.body[0] == '\r' && obj.reqq.body[1] == '\n')
                            obj.reqq.body = obj.reqq.body.substr(2, obj.reqq.body.length());
                        obj.reqq.body = obj.reqq.body.substr(obj.reqq.body.find("\r\n") + 2 , obj.reqq.body.size());
                        obj.reqq.file.write(obj.reqq.body.c_str(), obj.reqq.body.size() - 5);
                        obj.reqq.file.flush();
                        std::cout << "Done" << std::endl;
                        obj.reqq.statusCode = 201;
                        obj.r_done = 1;
                    }
    }

void Post::chunked_body2(client &obj){
    std::string filename;
    handleCgi obj2;
    location obj3 = obj.reqq.getMatchedLocation();

    std::map<std::string, std::string>::iterator iter = obj.reqq.headers.find("Content-Type");

        if(!obj.reqq.body.empty() && obj.reqq.body.find("\r\n0\r\n\r\n") != std::string::npos)
            obj.reqq.chunked_flag = 1;
        if(!obj.reqq.body.empty() && obj.reqq.chunked_flag == 1)
        {
            if(obj.reqq.flag == 0)
            {
                obj.reqq.load_extension();
                if(iter != obj.reqq.headers.end() && !iter->second.empty())
                {
                    obj.reqq.content_T = iter->second;
                    std::map<std::string, std::string>::iterator iter2 = obj.reqq.extension.find(iter->second);
                    std::string name = "Post_" + obj2.generateRandomFileName();
                    filename = "/"  + name + iter2->second;
                }
                else if(filename.empty())
                {
                    obj.reqq.content_T = "text/plain";
                    filename = "/" + obj2.generateRandomFileName() + ".txt";
                }
                filename = obj3.upload_path+ filename;
                obj.reqq.file.open(filename.c_str());
                obj.reqq.cgi_File = filename;
                obj.reqq.cgi_File2 = obj.reqq.path;
                obj.reqq.flag = 1;
            }
            if(obj.reqq.saver_count == 0)
            {
                if (obj.reqq.body[0] == '\r' && obj.reqq.body[1] == '\n')
                    obj.reqq.body = obj.reqq.body.substr(2, obj.reqq.body.length());
                obj.reqq.saver_count = obj.reqq.body.find("\r\n");
                obj.reqq.tmp = obj.reqq.saver_count + 2;
                if(obj.reqq.saver_count == 0)
                    throw Except();
                obj.reqq.getit = obj.reqq.body.substr(0, obj.reqq.saver_count);
                obj.reqq.to_de = hexa_to_num(obj.reqq.getit);
                obj.reqq.to_de2 += obj.reqq.to_de;
                obj.reqq.body = obj.reqq.body.substr(obj.reqq.tmp, obj.reqq.body.size());
            }
            if(obj.reqq.body.size() >= obj.reqq.to_de)
            {
                    obj.reqq.file.write(obj.reqq.body.c_str(), obj.reqq.to_de);
                    obj.reqq.body = obj.reqq.body.substr(obj.reqq.to_de, obj.reqq.body.length());
                    obj.reqq.file.flush();
                    obj.reqq.saver_count = 0;
            }
            if(obj.reqq.body.find("\r\n0\r\n\r\n") != std::string::npos)
            {
                obj.reqq.file.write(obj.reqq.body.c_str(), obj.reqq.body.size() - 5);
                obj.reqq.file.flush();
                obj.reqq.statusCode = 201;
                obj.reqq.responseContentLen = obj.reqq.to_de2;
                obj.r_done = 1;
            }
        }
        obj.reqq.chunked_flag = 0;
    }


void Post::Work_with_file(Request &obj){
   location capt = obj.getMatchedLocation();
    std::map<std::string, std::string> get = capt.cgi;
    std::map<std::string, std::string>::iterator iter = get.begin();
    std::string search;
    std::string tmp;
    int integ = 0;

    for(iter = get.begin(); iter != get.end(); iter++)
    {
        search = iter->first;
        search = "." + search;
        tmp = obj.path.rfind(search);
        if(tmp.size() != std::string::npos){
            integ = 1;
            break;
        }
    }
    if(integ == 1){
        method use;
        handleCgi cgi;
        if (use.loacationHasCgi(obj, cgi)){
            cgi.executeCgiBody(obj);
        }
    }
    else{
        //403 forbiden
        throw Except();
    }
}

