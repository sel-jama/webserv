/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:13:01 by sel-jama          #+#    #+#             */
/*   Updated: 2024/03/31 23:42:20 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Method.hpp"
#include "../Request/Request.hpp"

bool Method::loacationHasCgi(Request &req, handleCgi &cgi) {
    try{
        cgi.setScriptName(req.fileName);
        cgi.validateCgi(req);
        return true;
    }
    catch(const std::runtime_error &e){
        return false;
    }
    return true;
}

void Method::validateAll(Request &req) const{
    if (!(req.pathStatus.st_mode& S_IRUSR)){
        req.statusCode = 403;
        throw std::runtime_error("forbiden : permission denied");
    }
}

//handle GET Method
void Method::GetDataForClient(Request &req){
    defineResourceType(req); //file or dir
    // std::cout << "************ "<<type << std::endl;
    if (type == "file") {
        //if location does not have cgi
        validateAll(req);  //toFix
        if (loacationHasCgi(req, cgi)){
            std::cout << "CGI .... " << std::endl;
            cgi.executeCgiScript(req);
        }
        else{
            req.filePath = req.path;
            // content = readContent(req);
            req.responseContentLen =  req.pathStatus.st_size;
            std::cout <<"here " << req.responseContentLen<< std::endl;
        }
    }
    else
        handleDirectory(req);

    // req.responseContentType = getMimeType(req.fileName);
    // std::string content = readContent(req.fileName);


    this->response = content;
    // std::cout << this->response << std::endl;
    // if (send(clientSocket, this->response.c_str(), this->response.length(), 0) == -1)
    //     throw std::runtime_error("Error: Failed to send response to client");
    
}

std::string Method::readContent(Request &req){
    // std::cout << "got here" << std::endl;
    // req.path = "../error/page.html";
    std::ifstream file(req.path.c_str());
    if (!file.is_open())
    {
        req.statusCode = 500;
        throw std::runtime_error("set error page : failed to read requested content");
        std::cerr << "failed to read content" << std::endl;
    }
    // const size_t chunkSize = 1024; // 1 KB
    // char buffer[chunkSize];

    // if (!file.is_open()) {
    //     std::cerr << "Error: Failed to open file " << req.fileName << "\n";
    //     return "";
    // }
    std::ostringstream content;
    content << file.rdbuf();
    file.close();
    //content length is not set yet !!!!!!!
    // size_t contentSize = req.contentLength;
    // while (contentSize > 0) {
    //     file.read(buffer, chunkSize);
    //     content << buffer;
    //     memset(buffer, 0, chunkSize);
    //     contentSize -= chunkSize;
    // }
    // file.close();
    //set reading state to false 
    return content.str();
}

void Method::defineResourceType(Request &req){
    // const std::string uri = req.getUri();
    if (S_ISDIR(req.pathStatus.st_mode))
        type = "directory";
    else
        type = "file";
    // req.type = type;
}

void Method::handleDirectory(Request &req){
    handleCgi cgi;
    // size_t uriLength = req.getUri().length() - 1;
    // if (req.getUri().at(uriLength) == '/'){
        if (isDirHasIndexFiles(req) == false)
            autoIndexing(req);
        //no index files and no cgi
        // else if (req.getMatchedLocation().cgi.size() == 0)
        else{
            struct stat status;
            req.path += "/" + req.fileName;
            if (stat(req.path.c_str(), &status) != 0){
                req.statusCode = 500;
                throw std::runtime_error("stat failed");
            }
            if (S_ISDIR(status.st_mode))
                handleDirectory(req);
            else{
                validateAll(req);
                if (loacationHasCgi(req, cgi))
                    cgi.executeCgiScript(req);
                else
                    this->content = readContent(req);
            }
        }

    // }
    //else : make a 301 redirection to request uri with “/ addeed at the end

}

// void Method::handleClient(server &serve, Request &req, int &clientSocket) {
//     char buffer[BUFFER_SIZE];
//     retreiveRequestedResource(serve, req);
//     int bytes_received = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
//     if (bytes_received < 0) {
//         std::cerr << "Error in receiving data from client\n";
//         return ;
//     }
//     buffer[bytes_received] = '\0';

//     handle_get_request(clientSocket);
// }

bool Method::isDirHasIndexFiles(Request &req) const{
    DIR *dir;
    struct dirent *ent;

    // Open directory
    if ((dir = opendir(req.path.c_str()))) {
        // Iterate through directory entries
        while ((ent = readdir(dir)) != NULL) {
            std::string filename = ent->d_name;
            // size_t pos = filename.find('.');

            // filename.substr(0, pos);
            for (std::vector<std::string>::iterator it = req.matchedLocation.index.begin(); it != req.matchedLocation.index.end(); ++it){
                if (filename == *it)  { //compare with index files in location later
                    req.fileName = *it;
                    closedir(dir);
                    // req.path += filename;
                    return true;
                }
            }
        }
        closedir(dir);
    } else {
        // Unable to open directory
        req.statusCode = 500;
        std::cerr << "Unable to open directory: " << req.path << std::endl;
    }

    // No index file found
    return false;
}

void Method::autoIndexing(Request &req){
    (void)req;
    if (req.getMatchedLocation().autoindex == "on")
        directoryListing(req);
    else{
        req.statusCode = 403;
        throw std::runtime_error("403 Forbiden");
    }
}

void Method::directoryListing(Request &req){
    std::stringstream page;

    DIR* directory = opendir(req.path.c_str());
    if (directory == NULL)
    {
        req.statusCode = 500;
        throw std::runtime_error("Error opening directory");
    }

    page << "<html><head><title>Index of " << req.path.c_str() << "</title></head><body><h1>Index of " << req.path.c_str() << "</h1><ul>" << std::endl;

    req.fileName += req.fileName.back() == '/' ? "" : "/";

    struct dirent* entry;
    while ((entry = readdir(directory)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            page << "<li><a href=\"" << req.fileName + entry->d_name << "\">" << entry->d_name << "</a></li>" << std::endl;
        }
    }

    page << "</ul></body></html>" << std::endl;
    closedir(directory);
    this->content = page.str();
}



