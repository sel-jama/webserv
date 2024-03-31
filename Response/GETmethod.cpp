/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GETmethod.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:13:01 by sel-jama          #+#    #+#             */
/*   Updated: 2024/03/30 01:14:06 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GETmethod.hpp"

void HTTPServer::handleClient(int clientSocket) {
    char buffer[1024] = {0};
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead == -1) {
        std::cerr << "Error: Failed to read from client socket\n";
        return;
    }

    std::istringstream request(buffer);
    std::string method, uri, httpVersion;
    request >> method >> uri >> httpVersion;

    if (method != "GET") {
        std::cerr << "Error: Unsupported HTTP method\n";
        return;
    }

    // Remove leading '/' from URI
    if (uri.length() > 0 && uri[0] == '/') {
        uri = uri.substr(1);
    }

    std::string fileName = uri.empty() ? "index.html" : uri;
    std::string mimeType = getMimeType(fileName);
    std::string content = readFile(fileName);

    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n"
             << "Content-Type: " << mimeType << "\r\n"
             << "Content-Length: " << content.length() << "\r\n"
             << "\r\n"
             << content;

    if (send(clientSocket, response.str().c_str(), response.str().length(), 0) == -1) {
        std::cerr << "Error: Failed to send response to client\n";
    }
}

std::string HTTPServer::getMimeType(const std::string& fileName) {
    size_t dotPos = fileName.find_last_of('.');
    if (dotPos != std::string::npos) {
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

std::string HTTPServer::readFile(const std::string& fileName) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file " << fileName << "\n";
        return "";
    }

    std::ostringstream content;
    content << file.rdbuf();
    file.close();
    return content.str();
}

int sameUntilIndex(const std::string &uri, const std::string &locationName){
    size_t i = 0;
    while (uri[i] && locationName[i] && uri[i] == locationName[i])
        i++;
    return i;
}

const location &GETmethod::getMatchingLocation(server &serve, Request &req) {
    int counter;

    size_t i = 0;
    counter = sameUntilIndex(req.getUri(), serve.getLocations().at(i).location_name);
    i++;
    for (; i < serve.getLocations().size(); i++){
        if (sameUntilIndex(req.getUri(), serve.getLocations().at(0).location_name) > counter)
            counter = sameUntilIndex(req.getUri(), serve.getLocations().at(0).location_name);  
    }
    if (i >= serve.getLocations().size())
        throw std::runtime_error("404 Not found : No matching location");
    
    return serve.getLocations().at(i);
    
}

void GETmethod::retreiveRequestedResource(server &serve, Request &req) const{
    std::string path;
    const location &locate = getMatchingLocation(serve, req);
    //see the root of the location retrieved and join it with the uri then look for it using access
}

void GETmethod::handleClient(server &serve, Request &req, int &clientSocket) {
    char buffer[BUFFER_SIZE];
    retreiveRequestedResource(serve, req);
    int bytes_received = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received < 0) {
        std::cerr << "Error in receiving data from client\n";
        return ;
    }
    buffer[bytes_received] = '\0';

    handle_get_request(clientSocket);
}
