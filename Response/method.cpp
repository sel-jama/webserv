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
//handle GET method

void method::GetDataForClient(Request &req, int &clientSocket) {
    defineResourceType(req); //file or dir
    if (type == "file"){
        //if location does not have cgi
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
        std::cerr << "Error: Failed to send response to client\n";
    }
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
    
    std::ifstream file(req.path, std::ios::binary);
    const size_t chunkSize = 1024; // 1 KB
    char buffer[chunkSize];
    
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file " << req.fileName << "\n";
        return "";
    }

    std::ostringstream content;
    //content length is not set yet !!!!!!!
    size_t contentSize = req.contentLength;
    while (contentSize > 0) {
        file.read(buffer, chunkSize);
        content << buffer;
        memset(buffer, 0, chunkSize);
        contentSize -= chunkSize;
    }
    file.close();
    //set reading state to false 
    return content.str();
}

void method::defineResourceType(const Request &req) {
    const std::string uri = req.getUri();
    if (S_ISDIR(req.pathStatus.st_mode))
        type = "directory";
    else
        type = "file";
}

void method::handleDirectory(Request &req)  {
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
            if (filename == "index") {
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

void method::autoIndexing(Request &req) const {
    (void)req;
    // if (req.getMatchedLocation().autoindex == "on")
    //     directoryListing(req);
    // throw std::runtime_error("403 Forbiden");
}

// void method::directoryListing(Request &req) const{

//     const std::string& path, const std::string& host, int port;
//     std::vector<std::string> fileList = getFileList(path);
//     if (fileList.empty()) {
//         std::cerr << "Error: could not open [" << path << "]" << std::endl;
//         return "";
//     }

//     std::stringstream page;
//     page << "<!DOCTYPE html>\n"
//          << "<html>\n"
//          << "<head>\n"
//          << "    <title>Index of " << path << "</title>\n"
//          << "</head>\n"
//          << "<body>\n"
//          << "<h1>Index of " << path << "</h1>\n"
//          << "<ul>\n";

//     for (const std::string& file : fileList) {
//         page << "    <li><a href=\"http://" << host << ":" << port << "/" << file << "\">" << file << "</a></li>\n";
//     }

//     page << "</ul>\n"
//          << "</body>\n"
//          << "</html>\n";

//     return page.str();

// }
