/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:13:01 by sel-jama          #+#    #+#             */
/*   Updated: 2024/03/31 02:31:49 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GetHandler.hpp"

void GetHandler::GetDataForClient(const server &serve, Request &req, int &clientSocket) {
    defineResourceType(req);
    if (type == "file"){
        //if location does not have cgi
        readContent(req);
    }
    // std::string mimeType = getMimeType(req.fileName);
    // std::string content = readContent(req.fileName);

    // std::ostringstream response;
    // response << "HTTP/1.1 200 OK\r\n"
    //          << "Content-Type: " << mimeType << "\r\n"
    //          << "Content-Length: " << content.length() << "\r\n"
    //          << "\r\n"
    //          << content;

    if (send(clientSocket, response.str().c_str(), response.str().length(), 0) == -1) {
        std::cerr << "Error: Failed to send response to client\n";
    }
}

std::string GetHandler::getMimeType(const std::string& fileName) {
    size_t dotPos = fileName.find_last_of('.');
    if (dotPos != std::string::npos){
        std::string extension = fileName.substr(dotPos + 1);
        if (extension == "html" || extension == "htm") {
            return "text/html";
        } else if (extension == "css") {
            return "text/css";
        } else if (extension == "js") {
            return "application/javascript";
        } else if (extension == "jpg" || extension == "jpeg") {
            return "image/jpeg";
        } else if (extension == "png") {
            return "image/png";
        } else if (extension == "gif") {
            return "image/gif";
        } else if (extension == "txt") {
            return "text/plain";
        }
    }
    return "application/octet-stream";
}

std::string GetHandler::readContent(Request &req) {
    std::ifstream file(req.fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file " << req.fileName << "\n";
        return "";
    }

    std::ostringstream content;
    size_t contentSize = 0;
    while (contentSize != req.contentLength) {
        content << file.rdbuf();
        contentSize+=1024;
    }
    file.close();
    return content.str();
}

void GetHandler::defineResourceType(const Request &req) {
    const std::string uri = req.getUri();
    if (uri.at(uri.length() - 1) == '/')
        type = "directory";
    type = "file";
}

// void GetHandler::handleClient(server &serve, Request &req, int &clientSocket) {
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
