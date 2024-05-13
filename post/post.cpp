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
// #include "../Response/"

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

void Post::After_geting_resource(Request obj){
        Work_with_file(obj); 
}

void Post::support_upload(Request &obj)
{
    handleCgi obj2;
    int check = 0;
    std::string value;
    obj.load_extension();
    location obj3 = obj.getMatchedLocation();

                if(obj3.upload != "on" || (obj3.upload == "on" && obj3.upload_path.empty()))
                {
                    obj.statusCode = 403;
                    throw std::runtime_error("rja3");
                }
                if(!obj.headers["content-type"].empty() && !obj.extension[obj.headers["content-type"]].empty())
                {
                    obj.content_T = obj.headers["content-type"];
                    value = obj.headers["content-type"];
                    std::string name = "Post_" + obj2.generateRandomFileName();
                    obj.filename__ = "/"  + name + obj.extension[obj.headers["content-type"]];
                }
                location capt = obj.getMatchedLocation();
                std::string  ptr = capt.upload_path;
                if(ptr.empty())
                {
                    obj.statusCode = 500;
                    throw std::runtime_error("eror");
                }
                    const char* ptr2= obj.getMatchedLocation().upload_path.c_str();
                    check = access(ptr2, F_OK);
                    if (check == -1) {
                        obj.statusCode = 404;
                        throw std::runtime_error("eror");
                    }
                if(obj.filename__.empty())
                {
                    obj.content_T  = "text/plain";
                    obj.filename__ = "/Post_" + obj2.generateRandomFileName() + ".txt";
                }

                    obj.filename__ =  obj.getMatchedLocation().upload_path + obj.filename__;
                    // std::ofstream file(filename.c_str());
                    obj.file->open(obj.filename__.c_str());
                    obj.cgi_File = obj.filename__; 
                    obj.cgi_File2 = obj.path;
                    obj.path = obj.filename__; //
                    obj.flag2 = 1;

}   


void Post::body(client &obj){
    if(obj.reqq.matchedLocation.upload != "on" || (obj.reqq.matchedLocation.upload == "on" && obj.reqq.matchedLocation.upload_path.empty()))
    {
        obj.reqq.statusCode = 403;
        throw std::runtime_error("rja3");
    }
    if(!(obj.reqq.size_body >= obj.reqq.contentLength))
    {
        std::stringstream ss(obj.reqq.readRequest(obj));
        obj.reqq.file->write(ss.str().c_str(), ss.str().size());
        obj.reqq.size_body += ss.str().size();
    }
    if(obj.reqq.size_body >= obj.reqq.contentLength){
        obj.reqq.file->close();
        obj.reqq.responseContentLen = obj.reqq.body.length();
        obj.reqq.statusCode = 201;
        Work_with_file(obj.reqq);
        obj.r_done = 1;
    }
    else
    { 
        obj.r_done = 0;
    }
}

void Post::chunked_body(client &obj){
    std::string filename;
    handleCgi obj2;
    location obj3 = obj.reqq.getMatchedLocation();

    if(obj3.upload != "on" || (obj3.upload == "on" && obj3.upload_path.empty()))
    {
        obj.reqq.statusCode = 403;
        throw std::runtime_error("rja3");
    }
        if(obj.reqq.flag == 0)
        {
            obj.reqq.load_extension();
            if(!obj.reqq.headers["content-type"].empty() && !obj.reqq.extension[obj.reqq.headers["content-type"]].empty())
            {
                obj.reqq.content_T = obj.reqq.headers["content-type"];
                std::string iter2 = obj.reqq.extension[obj.reqq.headers["content-type"]];
                std::string name = "Post_" + obj2.generateRandomFileName();
                filename = "/"  + name + iter2;
            }
            else
            {
                obj.reqq.content_T = "text/plain";
                filename = "/" + obj2.generateRandomFileName() + ".txt";
            }
            filename = obj3.upload_path + filename;
            obj.reqq.file->open(filename.c_str());
            obj.reqq.cgi_File = filename; 
            obj.reqq.cgi_File2 = obj.reqq.path; 
            obj.reqq.flag = 1;
        }
        obj.reqq.body.append( obj.reqq.readRequest(obj));
        if(obj.reqq.saver_count == 0)
            {
                if (obj.reqq.body[0] == '\r' && obj.reqq.body[1] == '\n')
                    obj.reqq.body = obj.reqq.body.substr(2, obj.reqq.body.length());
                obj.reqq.saver_count = obj.reqq.body.find("\r\n");
                obj.reqq.tmp = obj.reqq.saver_count + 2;
                if(obj.reqq.saver_count == 0)
                {
                    obj.reqq.statusCode = 400;
                    throw  std::runtime_error("eror");
                }
                obj.reqq.getit = obj.reqq.body.substr(0, obj.reqq.saver_count);
                obj.reqq.to_de = hexa_to_num(obj.reqq.getit);
                obj.reqq.to_de2 += obj.reqq.to_de;
                obj.reqq.responseContentLen = obj.reqq.to_de2;
                if(static_cast<long long>(obj.reqq.responseContentLen) > obj.reqq.maxBodySize)
                {
                    obj.reqq.statusCode = 413;
                    if(std::remove(filename.c_str()) != 0)
                        obj.reqq.statusCode = 500;
                    throw std::runtime_error("eror");
                }
                obj.reqq.body = obj.reqq.body.substr(obj.reqq.tmp, obj.reqq.body.size());
            }
            if(obj.reqq.body.size() >= obj.reqq.to_de)
            {
                    obj.reqq.file->write(obj.reqq.body.c_str(), obj.reqq.to_de);
                    obj.reqq.body = obj.reqq.body.substr(obj.reqq.to_de, obj.reqq.body.length());
                    obj.reqq.file->flush();
                    obj.reqq.saver_count = 0;
                }
            if(obj.reqq.body.find("\r\n0\r\n\r\n") != std::string::npos)
                {
                    if (obj.reqq.body[0] == '\r' && obj.reqq.body[1] == '\n')
                        obj.reqq.body = obj.reqq.body.substr(2, obj.reqq.body.length());
                    obj.reqq.body = obj.reqq.body.substr(obj.reqq.body.find("\r\n") + 2 , obj.reqq.body.size());
                    obj.reqq.file->write(obj.reqq.body.c_str(), obj.reqq.body.size() - 5);
                    obj.reqq.file->flush();
                    obj.reqq.file->close();
                    obj.reqq.statusCode = 201;
                    obj.r_done = 1;

                }
    }

void Post::chunked_body2(client &obj){
    std::string filename;
    handleCgi obj2;
    location obj3 = obj.reqq.getMatchedLocation();

        if(!obj.reqq.body.empty() && obj.reqq.body.find("\r\n0\r\n\r\n") != std::string::npos)
        {
            obj.reqq.chunked_flag = 1;
        }
        if(!obj.reqq.body.empty() && obj.reqq.chunked_flag == 1)
        {
            if(obj.reqq.flag == 0)
            {
                obj.reqq.load_extension();
                if(!obj.reqq.headers["content-type"].empty() && !obj.reqq.extension[obj.reqq.headers["content-type"]].empty())
                {
                    obj.reqq.content_T = obj.reqq.headers["content-type"];
                    std::string iter2 = obj.reqq.extension[obj.reqq.headers["content-type"]];
                    std::string name = "Post_" + obj2.generateRandomFileName();
                    filename = "/"  + name + iter2;
                }
                else if(filename.empty())
                {
                    obj.reqq.content_T = "text/plain";
                    filename = "/" + obj2.generateRandomFileName() + ".txt";
                }
                filename = obj3.upload_path+ filename;
                obj.reqq.file->open(filename.c_str());
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
                if(obj.reqq.saver_count == 0){
                    obj.reqq.statusCode = 400;
                    throw std::runtime_error("eror");
                }
                obj.reqq.getit = obj.reqq.body.substr(0, obj.reqq.saver_count);
                obj.reqq.to_de = hexa_to_num(obj.reqq.getit);
                obj.reqq.to_de2 += obj.reqq.to_de;
                if(static_cast<long long>(obj.reqq.responseContentLen) > obj.reqq.maxBodySize)
                {
                    obj.reqq.statusCode = 413;
                    if(std::remove(filename.c_str()) != 0)
                        obj.reqq.statusCode = 500;
                    throw std::runtime_error("eror");
                }
                obj.reqq.body = obj.reqq.body.substr(obj.reqq.tmp, obj.reqq.body.size());
            }
            if(obj.reqq.body.size() >= obj.reqq.to_de)
            {
                    obj.reqq.file->write(obj.reqq.body.c_str(), obj.reqq.to_de);
                    obj.reqq.body = obj.reqq.body.substr(obj.reqq.to_de, obj.reqq.body.length());
                    obj.reqq.file->flush();
                    obj.reqq.saver_count = 0;
            }
            if(obj.reqq.body.find("\r\n0\r\n\r\n") != std::string::npos)
            {
                obj.reqq.file->write(obj.reqq.body.c_str(), obj.reqq.body.size() - 5);
                obj.reqq.file->flush();
                obj.reqq.statusCode = 201;
                obj.reqq.responseContentLen = obj.reqq.to_de2;
                obj.r_done = 1;
            }
        }
    }


void Post::Work_with_file(Request &obj){
   location capt = obj.getMatchedLocation();
    std::map<std::string, std::string> get = capt.cgi;
    std::map<std::string, std::string>::iterator iter = get.begin();
    std::string search;
    unsigned long tmp1 = 0;
    int integ = 0;

    (void)tmp1;
    for(iter = get.begin(); iter != get.end(); iter++)
    {
        search = iter->first;
        search = "." + search;
        tmp1 = obj.path.find(search);
        if(obj.cgi_File2.find(search) != std::string::npos){
            integ = 1;
            break;
        }
    }
    if(integ == 1){
        Method use;
        handleCgi cgi;
        if (use.loacationHasCgi(obj, cgi)){
            cgi.executeCgiBody(obj);
        }
    }
}

