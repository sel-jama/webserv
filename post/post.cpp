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
            std::cout << "helo ->>>>>>>>" << std::endl;
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
    if(static_cast<int>(obj.reqq.body.length()) >= obj.reqq.contentLength){
        std::cout << "reading BODY DONE HERE " << std::endl;
        obj.r_done = 1;
    }
    else
    {
        // std::cout << "olah tantqad "<< std::endl;
        obj.r_done = 0;
    }
}

std::ofstream Request::file;

void Post::chunked_body(client &obj){

    // std::string capt;
    // std::cout << "heloooooooo" << std::endl;
    // if(!obj.reqq.r_s.empty())
    //     capt = obj.reqq.r_s;
    // capt.append(obj.reqq.body);
    // std::string capt = obj.readRequest(obj.fdsock3);
    // capt = capt + obj.bodySaver;
    // int counter = 0;
    // int length = 0;
    // int save = 0;
    // int begin = 0;
    // char *ptr = const_cast<char *>(obj.reqq.body.c_str());
    // std::string capt = obj.reqq.body;
        // std::cout << "wlah ta jit" << std::endl;

    // size_t index = 0;

    // while ( index < capt.size())
    // {
    //     if (obj.reqq.flag == 0)
    //     {
    //         while (index < capt.size() && capt[index] != '\r')
    //             obj.reqq.getit += capt[index++];
    //         if (capt[index] == '\r')
    //         {
    //             obj.reqq.flag ++;
    //             obj.reqq.to_de = hexa_to_num(obj.reqq.getit);
    //             std::cout << "number =========>>>>>>>>> " << obj.reqq.getit << std::endl;
    //         }
    //     }
    //     else if (obj.reqq.flag == 1 || obj.reqq.flag == 5)
    //     {
    //         if (capt[index] == '\r')
    //         {
    //             index ++;
    //             obj.reqq.flag++;
    //         }
    //         // else bad request
    //     }
    //     else if (obj.reqq.flag == 2 || obj.reqq.flag == 6)
        if(obj.reqq.flag == 0)
        {
            // std::cout << "created" << std::endl;
            obj.reqq.file.open(obj.reqq.path + "/" + "uhm.mp4");
            obj.reqq.flag = 1;
        }
    //     {
    //         if (capt[index] == '\n')
    //         {
    //             index ++;
    //             obj.reqq.flag++;
    //         }
    //         // else bad request
    //         if (obj.reqq.flag == 7)
    //         {
    //             if (hexa_to_num(obj.reqq.getit) == 0)
    //             {
    //                 obj.reqq.file.close();
    //                 // obj.reqq.r_done = 1;
    //                 std::cout << "end end end" << std::endl;
    //             }
    //             obj.reqq.getit.clear();
    //             obj.reqq.flag = 0;
    //             obj.reqq.body = obj.reqq.readRequest(obj.ssocket);
    //             Post::chunked_body(obj);
    //         }
    //     }
    //     else if (obj.reqq.flag == 3)
    //     {
    //         long size_write;
    //         if (obj.reqq.to_de < capt.size() - index)
    //             size_write = obj.reqq.to_de;
    //         else {
    //             size_write = capt.size() - index;
    //         }            
    //         obj.reqq.file.write(capt.substr(index, size_write).c_str(), size_write);
    //         obj.reqq.file.flush();
    //         obj.reqq.to_de -= size_write;
    //         index += size_write;
    //         if (obj.reqq.to_de <= 0)
    //             obj.reqq.flag++;
    //     }
    // }

    // while(1)
    // {
    //     obj.reqq.body.append(obj.reqq.readRequest(obj.ssocket));
        // if(obj.reqq.saver_count == 0)
        // {
        //     std::cout <<  "dkhelt had lmra " <<std::endl;
        //     obj.reqq.saver_count = capt.find("\r\n");
        //     obj.reqq.tmp = obj.reqq.saver_count + 2;
        //     if(obj.reqq.saver_count == 0)
        //         throw Except();
        //     obj.reqq.flag = 1;
        //     obj.reqq.getit = capt.substr(0, obj.reqq.saver_count);
        //     obj.reqq.to_de = hexa_to_num(obj.reqq.getit);
        //     std::cout << "number =========>>>>>>>>> " << obj.reqq.to_de << std::endl;
        // }
        // if(obj.reqq.flag == 4 && capt.find("\r", obj.reqq.saver_count) != 0)
        // {
        //     save = capt.find("\r", obj.reqq.saver_count);
        //     obj.reqq.getit = capt.substr(obj.reqq.saver_count, save);
        //     obj.reqq.to_de = hexa_to_num(obj.reqq.getit);
        //     obj.reqq.flag = 1;
        // }
        obj.reqq.body.append( obj.reqq.readRequest(obj.ssocket));
        // if(obj.reqq.flag == 1 && capt.find("\n", obj.reqq.saver_count) != 0)
        // {
        //     obj.reqq.saver_count = capt.find("\n", obj.reqq.saver_count);
        //     obj.reqq.saver_count++;
            
        //     obj.reqq.flag = 2;
        // }
       
            if(obj.reqq.saver_count == 0)
            {
                // std::cout <<  "dkhelt had lmra " <<std::endl;
                if (obj.reqq.body[0] == '\r' && obj.reqq.body[1] == '\n')
                    obj.reqq.body = obj.reqq.body.substr(2, obj.reqq.body.length());
                // std::cout << obj.reqq.body.substr(0, 20) << "jellol" << std::endl;
                obj.reqq.saver_count = obj.reqq.body.find("\r\n");

                // std::cout << obj.reqq.saver_count << std::endl;
                obj.reqq.tmp = obj.reqq.saver_count + 2;
                if(obj.reqq.saver_count == 0)
                    throw Except();
                // obj.reqq.flag = 1;
                obj.reqq.getit = obj.reqq.body.substr(0, obj.reqq.saver_count);
                // std::cout << "getit" << obj.reqq.getit << std::endl;
                obj.reqq.to_de = hexa_to_num(obj.reqq.getit);
                obj.reqq.body = obj.reqq.body.substr(obj.reqq.tmp, obj.reqq.body.size());
                // std::cout << "number =========>>>>>>>>> " << obj.reqq.to_de << std::endl;
                // index = obj.reqq.saver_count;
            }
            // obj.reqq.r_s = obj.reqq.body[index];
            // std::cout << obj.reqq.saver_count <<  " " << obj.reqq.body.size()  << " " << obj.reqq.to_de<< std::endl;
            if(obj.reqq.body.size() >= obj.reqq.to_de)
            {
                // save = obj.reqq.body.find("\r", obj.reqq.saver_count);
                // obj.reqq.getit = obj.reqq.body.substr(obj.reqq.saver_count, save - 4);
                // if(static_cast<int>(obj.reqq.getit.length()) != obj.reqq.to_de)
                //     throw Except();    
                // if(obj.reqq.body[index + 1] == '\n')
                // {    
                    // std::cout << obj.reqq.saver_count  << "<<<<<<<<<<+++++++++++==============================="<< std::endl;
                    obj.reqq.file.write(obj.reqq.body.c_str(), obj.reqq.to_de);
                    // std::cout << "hello" << std::endl;
                    obj.reqq.body = obj.reqq.body.substr(obj.reqq.to_de, obj.reqq.body.length());
                    // std::cout << "body len: " << obj.reqq.body.length() << std::endl;
                    obj.reqq.file.flush();
                    // obj.reqq.to_de = 0;

                    obj.reqq.saver_count = 0;
                    // obj.r_done = 1;
                // }
                }
            // if(obj.reqq.body.find("0\r\n\r\n") != std::string::npos)
                if(obj.reqq.body.find("\r\n0\r\n\r\n") != std::string::npos)
                    {


                        while (!obj.reqq.body.empty() && (isspace(obj.reqq.body.front()) || obj.reqq.body.front() == '1' || obj.reqq.body.front() == '5')) obj.reqq.body.erase(0, 1);
                        while (!obj.reqq.body.empty() && (isspace(obj.reqq.body.back()) || iscntrl(obj.reqq.body.back()) || obj.reqq.body.back() == '0')) obj.reqq.body.pop_back();
                        while (!obj.reqq.body.empty() && (isspace(obj.reqq.body.back()) || iscntrl(obj.reqq.body.back()))) obj.reqq.body.pop_back();
                        obj.reqq.file.write(obj.reqq.body.c_str(), obj.reqq.body.size());
                        obj.reqq.file.flush();
                        // std::cout  << obj.reqq.body<< std::endl;
                        // std::string str = obj.reqq.body.substr(obj.reqq.body.find("\r\n"));
                        // std::cout << "here " << str << std::endl;
                        // for (size_t i = 0; i < str.size(); i++){
                        //     if ((str[i] > 0 && str[i] < 31) || str[i] == 127)
                        //         std::cout << (int)str[i] << std::endl;
                        //     else
                        //         std::cout << str[i] << std::endl;;
                        // }
                        std::cout << "Done" << std::endl;
                        obj.r_done = 1;
                    }
            // std::cout << "tqrat hadi : "<<obj.reqq.body.find("0\r\n\r\n") << std::endl;
            // std::cout <<  "\\ "<< obj.reqq.body.size() <<   obj.reqq.to_de << std::endl;
            // if(obj.reqq.body.find("0\r\n\r\n") && (obj.reqq.body.size() >= obj.reqq.to_de))
            // {
            //     std::cout << obj.reqq.body.substr(0, obj.reqq.body.find("0\r\n\r\n")).c_str() << std::endl;
                
            //         // break;
            //     // if(obj.reqq.body[save - 1] && capt[save - 1] == 0)
            //     // obj.reqq.saver_count = 0;
            //     // obj.reqq.flag = 0;
            // }
            // if(obj.reqq.flag == 4 && capt.find("\r", obj.reqq.saver_count) == 1)
            // {
            //     save = capt.find("\r", obj.reqq.saver_count);
            //     obj.reqq.getit = capt.substr(obj.reqq.saver_count, save);
            //     if(obj.reqq.getit[0] == 0)
            //     {
            //         obj.r_done = 1;
            //         std::cout << "======================================================================>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<std::endl;
            //     }
            //     if(obj.reqq.body.length() == obj.reqq.to_de)
            //         obj.reqq.flag = 4;
            //     else
            //         obj.reqq.flag = 1;
            //     obj.reqq.saver_count++;
            // }
            // if(obj.reqq.body.length() == obj.reqq.to_de)
            // {
            //     obj.r_done = 1;
            //     std::cout << "======================================================================>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<std::endl;
            // }
            // if(obj.reqq.saver_count != 0)
                // obj.reqq.saver_count++;
        //     index++;
        // }
    // if(obj.r_done != 0)
    // {
    //     obj.reqq.body = obj.reqq.readRequest(obj.ssocket);
    //     Post::chunked_body(obj);
    // }
        // if(counter == 0)
        //     begin = capt.find("/r");
        // else
        // {
        //     save = begin;
        //     if(save == 0)
        //         break;
        //     begin = capt.find("/r\n", begin);
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

