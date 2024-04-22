/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:13:01 by sel-jama          #+#    #+#             */
/*   Updated: 2024/03/31 23:42:20 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "method.hpp"
#include "../Request/Request.hpp"
//handle GET method

void method::validateAll(Request &req) const{
    if (!(req.pathStatus.st_mode& S_IRUSR))
        throw std::runtime_error("forbiden : permission denied");
}

void method::GetDataForClient(Request &req, int &clientSocket) {
    defineResourceType(req); //file or dir
    if (type == "file") {
        //if location does not have cgi
        validateAll(req);
        content = readContent(req);
    }
    else
        handleDirectory(req);
    // std::string mimeType = getMimeType(req.fileName);
    // std::string content = readContent(req.fileName);

    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n"
            //  << "Content-Type: " << mimeType << "\r\n"
             << "Content-Length: " << content.length() << "\r\n"
             << "\r\n"
             << content;

    this->response = response.str();
    // std::cout << this->response << std::endl;
    if (send(clientSocket, response.str().c_str(), response.str().length(), 0) == -1) {
        throw std::runtime_error("Error: Failed to send response to client");
    }
}

void method::setErrorPages(){
    //html error pages
    errorPage[400] = "Bad Request";
    errorPage[401] = "Unquthorized";
    errorPage[403] = "Forbidden";
    errorPage[404] = "Not Found";
    errorPage[405] = "Method Not Allowed";
    errorPage[406] = "Not Acceptable";
    errorPage[407] = "Proxy Authentication Required";
    errorPage[408] = "Request Timeout";
    errorPage[409] = "Conflict";
    errorPage[410] = "Gone";
    errorPage[411] = "Length Required";
    errorPage[412] = "Precondition Failed";
    errorPage[413] = "Request Entity Too Large";
    errorPage[414] = "Request-URI Too Long";
    errorPage[415] = "Unsupported Media Type";
    errorPage[416] = "Requested Range Not Satisfiable";
    errorPage[417] = "Expectation Failed";
    errorPage[500] = "Internal Server Error";
    errorPage[501] = "Not Implemented";
    errorPage[502] = "Bad Gateway";
    errorPage[503] = "Service Unavailable";
}
// // std::string method::getMimeType(const std::string& fileName) {
// //     size_t dotPos = fileName.find_last_of('.');
// //     if (dotPos != std::string::npos){
// //         std::string extension = fileName.substr(dotPos + 1);
// //         if (extension == "html" || extension == "htm") {
// //             return "text/html";
// //         } else if (extension == "css") {
// //             return "text/css";
// //         } else if (extension == "js") {
// //             return "application/javascript";
// //         } else if (extension == "jpg" || extension == "jpeg") {
// //             return "image/jpeg";
// //         } else if (extension == "png") {
// //             return "image/png";
// //         } else if (extension == "gif") {
// //             return "image/gif";
// //         } else if (extension == "txt") {
// //             return "text/plain";
// //         }
// //     }
// //     return "application/octet-stream";
// // }

std::string method::readContent(Request &req){
    
    std::ifstream file(req.path.c_str());
    if (file.fail())
        throw std::runtime_error("set error page : failed to read requested content");
    // const size_t chunkSize = 1024; // 1 KB
    // char buffer[chunkSize];

    // if (!file.is_open()) {
    //     std::cerr << "Error: Failed to open file " << req.fileName << "\n";
    //     return "";
    // }

    std::ostringstream content;
    content << file.rdbuf();
    //content length is not set yet !!!!!!!
    // size_t contentSize = req.contentLength;
    // while (contentSize > 0) {
    //     file.read(buffer, chunkSize);
    //     content << buffer;
    //     memset(buffer, 0, chunkSize);
    //     contentSize -= chunkSize;
    // }
    file.close();
    //set reading state to false 
    return content.str();
}

void method::defineResourceType(const Request &req){
    const std::string uri = req.getUri();
    if (S_ISDIR(req.pathStatus.st_mode))
        type = "directory";
    else
        type = "file";
}

void method::handleDirectory(Request &req){
    size_t uriLength = req.getUri().length() - 1;
    if (req.getUri().at(uriLength) == '/'){
        if (isDirHasIndexFiles(req) == false)
            autoIndexing(req);
        //no index files and no cgi
        else if (req.getMatchedLocation().cgi.size() == 0)
            this->content = readContent(req);

    }
    //else : make a 301 redirection to request uri with “/ addeed at the end

}

// void method::handleClient(server &serve, Request &req, int &clientSocket) {
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

bool method::isDirHasIndexFiles(Request &req) const{
    DIR *dir;
    struct dirent *ent;

    // Open directory
    if ((dir = opendir(req.path.c_str()))) {
        // Iterate through directory entries
        while ((ent = readdir(dir)) != NULL) {
            std::string filename = ent->d_name;
            size_t pos = filename.find('.');

            filename.substr(0, pos);
            if (filename == "index")  { //compare with index files in location later
                closedir(dir);
                return true;
            }
        }
        closedir(dir);
    } else {
        // Unable to open directory
        std::cerr << "Unable to open directory: " << req.path << std::endl;
    }

    // No index file found
    return false;
}

void method::autoIndexing(const Request &req){
    (void)req;
    if (req.getMatchedLocation().autoindex == "on")
        directoryListing(req);
    throw std::runtime_error("403 Forbiden");
}

void method::directoryListing(const Request &req){
    std::stringstream page;

    DIR* directory = opendir(req.path.c_str());
    if (directory == NULL)
        throw std::runtime_error("serve page: Error opening directory");

    page << "<html><head><title>Index of " << req.path.c_str() << "</title></head><body><h1>Index of " << req.path.c_str() << "</h1><ul>" << std::endl;

    struct dirent* entry;
    while ((entry = readdir(directory)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            page << "<li><a href=\"" << entry->d_name << "\">" << entry->d_name << "</a></li>" << std::endl;
        }
    }

    page << "</ul></body></html>" << std::endl;
    closedir(directory);
    this->autoindexPage = page.str();
}



