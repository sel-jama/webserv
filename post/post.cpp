#include "post.hpp"
#include "../Request/Request.hpp"
#include <sstream>
#include <string.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
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

void Post::get_Request_resource(Request obj)
{
    struct stat buffer;
    location resource = obj.getMatchedLocation();
    std::string url = obj.getUri();
    std::string data = Body;
    std::string Encoding;
    std::map<std::string, std::string>::const_iterator value = obj.getHeaders().find("Transfer-Encoding"); 
    Encoding = value->second;
    const char *ptr = path.c_str();
    int check = access(ptr, F_OK);
    if (check  == -1) {
        throw Except();
    }
    // int val = stat(ptr, &buffer);
    if(S_ISDIR(buffer.st_mode))
       Type = "Directory";
    else
        Type = "File";
}

void Post::After_geting_resource(Request obj){
    if (Type == "File")
        Work_with_file(obj); 
    else
        Work_with_Directory(obj);
}

void Post::support_upload(Request &obj){
    int check = 0;
                    // std::cout << "......................" << std::endl;
    std::cout << "+++++++++ Im writing " << std::endl;
    location capt = obj.getMatchedLocation();
    std::string  ptr = capt.upload_path;
    if(ptr.empty())
        throw Except();
    else
    {
        const char* ptr2= ptr.c_str();
        check = access(ptr2, F_OK);
        std::cout << ptr2 << std::endl;
        if (check == -1) {
            // std::cout << "+++++++++ Im writing " << std::endl;
            throw Except();
        }
        else {
                std::ofstream file(ptr + "/" + "file.txt");
                obj.path = ptr + "/" + "file.txt";
                if (file.is_open() == true)
                {
                    // if (static_cast<int>(Body.length()) != obj.contentLength) {
                    //     throw Except(); 
                    // }
                    std::cout << "ja hnaya" << std::endl;
                    file << obj.body << std::endl;
                    file.close();
                    // std::cout << obj.body << std::endl;
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
    std::cout << "----------------- reading " << std::endl;
    obj.reqq.body.append(obj.reqq.readRequest(obj.ssocket));
        // std::cout << "bodyyy ************ " << obj.reqq.body << std::endl; 
    std::cout << "THIS IS THE BODY LEN " <<static_cast<int>(obj.reqq.body.length()) << std::endl;
    std::cout << "THIS IS CONTENT LENGTH " << obj.reqq.contentLength << std::endl;
    if(static_cast<int>(obj.reqq.body.length()) >= obj.reqq.contentLength ){
        std::cout << "reading BODY DONE HERE " << std::endl;
        obj.r_done = 1;
    }
    else
        obj.r_done = 0;
}

void Post::chunked_body(client &obj){
    std::string capt("");
    int check = 0;

    if(obj.reqq.readBody)
    {
        std::cout << "hselty" << std::endl;
        
        obj.reqq.body.append( obj.reqq.readRequest(obj.ssocket));
        capt = obj.reqq.body;
    }
    location get = obj.reqq.getMatchedLocation();
    std::string  ptr = get.upload_path;
    if(ptr.empty())
        throw Except();
    const char* ptr2= ptr.c_str();
    check = access(ptr2, F_OK);
    std::cout << ptr2 << std::endl;
    if (check == -1) {
            // std::cout << "+++++++++ Im writing " << std::endl;
        throw Except();
    }
    std::cout << "hello" << std::endl;
    std::ofstream file(ptr + "/" + "file.txt", std::ios::app);
        // obj.reqq.path = ptr + "/" + "file.txt";
        // if (file.is_open() == true)
        //      {
        //         // if (static_cast<int>(Body.length()) != obj.contentLength) {
        //         //     throw Except(); 
        //         // }
        //         std::cout << "ja hnaya" << std::endl;
        //         file << obj.reqq.body << std::endl;
        //         file.close();
        //         // std::cout << obj.body << std::endl;
        //     }
        //     else
        //         throw Except(); 
    
    // std::string capt = obj.readRequest(obj.fdsock3);
    // capt = capt + obj.bodySaver;
    // int counter = 0;
    // int length = 0;
    int save = 0;
    // int begin = 0;
    // char *ptr = const_cast<char *>(obj.reqq.body.c_str());
    // std::string capt = obj.reqq.body;
    if(capt.empty() && !obj.reqq.readBody)
    {
        for(size_t i=0; i < obj.reqq.body.size(); i++){
        if (obj.reqq.body[i] < 32 || obj.reqq.body[i] >126)
            std::cout << static_cast<int>(obj.reqq.body[i]) ;
        else
            std::cout << obj.reqq.body[i];
    }
        std::cout << "hello an f empty()" << std::endl;
        capt = obj.reqq.body;
    }

    std::string getit;
        std::cout << "wlah ta jit" << std::endl;
        if(obj.reqq.saver_count == 0)
        {
            obj.reqq.saver_count = capt.find('\r');
            obj.reqq.flag = 1;
            getit = capt.substr(0, obj.reqq.saver_count);
            obj.reqq.to_de = hexa_to_num(getit);
            std::cout << "number =========>>>>>>>>> " << obj.reqq.to_de << std::endl;
        }
        if(obj.reqq.flag == 4 && capt.find('\r', obj.reqq.saver_count) != 0)
        {
            save = capt.find('\r', obj.reqq.saver_count);
            getit = capt.substr(obj.reqq.saver_count, save);
            obj.reqq.to_de = hexa_to_num(getit);
            obj.reqq.flag = 1;
        }
        if(obj.reqq.flag == 1 && capt.find('\n', obj.reqq.saver_count) != 0)
        {
            std::cout << "ana f newline 1" << std::endl;
            obj.reqq.saver_count = capt.find('\n', obj.reqq.saver_count);
            std::cout << obj.reqq.saver_count << " baqi f newline"<< std::endl;
            obj.reqq.saver_count++;
            obj.reqq.flag = 2;
        }
        if(obj.reqq.flag == 2 && capt.find('\r', obj.reqq.saver_count) != 0)
        {
            // obj.reqq.saver_count = obj.reqq.saver_count + 2;
            save = capt.find('\r', obj.reqq.saver_count);
            getit = capt.substr(obj.reqq.saver_count, save - 4);
            
            std::cout << obj.reqq.body.length() << getit.length() << " ok"<< std::endl;

                // if(static_cast<int>(getit.length()) != obj.reqq.to_de)
                //     throw Except();    
            
            file << getit;
            // obj.reqq.body.append(getit);
            obj.reqq.flag = 3;
        }
        if(obj.reqq.flag == 3 && capt.find('\n', obj.reqq.saver_count) != 0)
        {
            obj.reqq.saver_count = capt.find('\n', obj.reqq.saver_count);
            obj.reqq.saver_count++;
            obj.reqq.flag = 4;
        }
        if(obj.reqq.flag == 4 && capt.find('\r', obj.reqq.saver_count) == 1)
        {
            save = capt.find('\r', obj.reqq.saver_count);
            getit = capt.substr(obj.reqq.saver_count, save);
            if(getit.empty())
                obj.r_done = 1;
            if(static_cast<int>(obj.reqq.body.length()) == obj.reqq.to_de)
                obj.reqq.flag = 4;
            else
                obj.reqq.flag = 1;
        }
        // if(counter == 0)
        //     begin = capt.find("/r");
        // else
        // {
        //     save = begin;
        //     if(save == 0)
        //         break;
        //     begin = capt.find("/r/n", begin);
        // }
        // if(counter % 2 == 0)
        // {
        //     std::string to_h = capt.substr(save, begin);
        //     length = hexa_to_num(to_h);
        // }
        // else
        // {
        //     std::string string = capt.substr(save, begin);
        //     if(length != static_cast<int>(string.length()))  
        //         throw Except();
        //     if(string.empty())
        //         break;
        //     Body = Body + string;
        // }
        // counter++;
    }

void Post::Work_with_file(Request obj){
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
        tmp = obj.fileName.find(search);
        if(!tmp.empty()){
            integ = 1;
            break;
        }
    }
    if(integ == 1){
        
    }
    else{
        //403 forbiden
        throw Except();
    }
}

