/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 15:33:33 by sel-jama          #+#    #+#             */
/*   Updated: 2024/03/29 00:57:29 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(){}

Request::~Request(){}

const std::string& Request::getMethod() const{
    return this->method;
}

const std::string& Request::getUri() const{
    return this->uri;
}

const std::string& Request::getBody() const{
    return this->body;
}

const std::map<std::string, std::string>& Request::getHeaders() const{
    return this->headers;
}

std::string readRequest(int fdSocket) {
    char buffer[BUFFER_SIZE];
    int bytes = read(fdSocket, buffer, sizeof(buffer));
    if (bytes < 0)
        throw std::runtime_error("Error reading request from socket");
    std::string request(buffer);
    return request;
}

// Function to parse HTTP request
void Request::requestPartitioning(Request &saver, const std::string& request) {
    std::istringstream iss(request);

    iss >> saver.method >> saver.uri;
    // Parse header
    std::string headerLine;
    while (std::getline(iss, headerLine) && headerLine != "\r") {
        size_t pos = headerLine.find(':');
        if (pos != std::string::npos) {
            std::string key = headerLine.substr(0, pos);
            std::string value = headerLine.substr(pos + 1);
            
            while (!key.empty() && isspace(key.front())) key.erase(0, 1);
            while (!key.empty() && isspace(key.back())) key.pop_back();
            while (!value.empty() && isspace(value.front())) value.erase(0, 1);
            while (!value.empty() && isspace(value.back())) value.pop_back();
            saver.headers[key] = value;
        }
    }

    // Parse body if Content-Length header is present
    std::map<std::string, std::string>::const_iterator it = saver.headers.find("Content-Length");
    if (it != saver.headers.end()) {
        int contentLength = atoi(it->second.c_str());
        char bodyBuffer[contentLength + 1];
        iss.read(bodyBuffer, contentLength);
        bodyBuffer[contentLength] = '\0';
        saver.body = bodyBuffer;
    }
}

void Request::isReqWellFormed(Request &req) const{
    ParseRequest parse;

    parse.parseMethod(req.method);
    parse.parseHeaders(req.headers, req.method);
    parse.parseUri(req.uri);
    
    /*if => Request body larger than client max body size in config file*/

}


bool Request::allowedMethod(location& location) const {
    std::map<std::string, int>::iterator it = location.http_methods.begin();
    for (; it != location.http_methods.end(); it++){
        if (it->first == this->method)
            return true;
    }
    return false;
}

void Request::locateMatchingRequestURI(const server& use) const {
    std::vector<location> locations;
    std::string matchedLocation;
    size_t i;

    locations = use.getLocations();
    for (i = 0; i < locations.size(); i++) {
        if (this->uri == locations[i].location_name) {
            matchedLocation = locations[i].location_name;
            break;
        }
    }
    if (matchedLocation.empty())
        throw std::runtime_error("404 Not Found : Matching uri location not found");
    if (!allowedMethod(locations[i]))
        throw std::runtime_error("405 Method Not Allowed");
}

//start here
void    Request::getCheckRequest(const server &serve, int &fdSock){
    std::string reqStr;
    Request reqObj;

    reqStr = readRequest(fdSock);
    requestPartitioning(reqObj, reqStr);
    try {
        isReqWellFormed(reqObj);
        locateMatchingRequestURI(serve);
    }
    catch(std::runtime_error &e){
        std::cout << e.what() << std::endl;
    }
}