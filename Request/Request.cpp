/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 15:33:33 by sel-jama          #+#    #+#             */
/*   Updated: 2024/04/13 15:22:45 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(client &user) : method(""), uri(""), version(""), user(user){
}

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

// using namespace std;
// char *bin2hex(const unsigned char *input, size_t len){
//     char *res;
//     std::string hexa = "0123456789QBCDEF";

//     const char *hex = hexa.c_str();

//     if (!input || len <= 0)
//         return NULL;

//     int reslen = len * 3 + 1;
//     // res = new char[reslen];
//     res = (char*)malloc(reslen);
//     bzero(res, reslen);

//     for(size_t i =0; i<len; i++){
//         res[i*3]=hex[input[i] >> 4];
//         res[(i*3)+1] = hex[input[i] & 0x0F];
//         res[(i*3)+2] = ' ';
//     }   
//     return res;
// }

// std::string Request::readRequest(int &fdSocket){
//     // char buffer[BUFFER_SIZE] = {0};
//     std::stringstream buff("");

//     // // Use recv to read from the socket
//     // int bytesRead = recv(fdSocket, buffer, sizeof(buffer), 0);
//     // if (bytesRead == -1)
//     //     throw std::runtime_error("Error reading request from socket");

//     uint8_t recvline[BUcutOffBodySegmentFFER_SIZE+1];
//     int n;
//     while (( n = read(fdSocket, recvline, BUFFER_SIZE-1) ) > 0){
//             // fprintf(stdout, "\n%s\n\n%s", bin2hex(recvline, n), recvline);

//             //detect end of message (change it later)
//             if (recvline[n-1] == '\n')
//                 break;
//             memset(recvline, 0, BUFFER_SIZE);
//             buff << recvline << "\n";
            
//         }
//         if (n < 0){
//             std::cerr << "read failed" << std::endl;
//             exit(EXIT_FAILURE);
//         }
    
//     std::string request(buff.str());
//     return request;
// }

void Request::(std::string &request){
    size_t pos = request.find("\n\r\n\r");

    if(pos != std::string::npos){
        bodySaver = request.substr(pos);
        // int num = request.length() - pos;
        request.erase(pos, request.length() - 1);
    }
}

std::string Request::readRequest(int &fdSocket) {
    std::stringstream buff("");

    char recvline[BUFFER_SIZE];
    while ((readbytes = read(fdSocket, recvline, BUFFER_SIZE)) > 0){
        if (strstr(recvline, "\n\r\n\r"))  //change later >> break when content len is all served
            break;
        buff << recvline;
        memset(recvline, 0, BUFFER_SIZE);
    }

    if (readbytes < 0)
        throw std::runtime_error("Error reading from socket: socket failed");
        
    else if (readbytes == 0)
        throw std::runtime_error("Peer closed the connection");
    
    std::string request(buff.str());
    cutOffBodySegment(request);
    return request;
}

// Function to parse HTTP request
void Request::requestPartitioning(Request &saver, std::string& request) {
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
        contentLength = atoi(it->second.c_str());
        // char bodyBuffer[contentLength + 1];
        // iss.read(bodyBuffer, contentLength);
        // bodyBuffer[contentLength] = '\0';
        // saver.body = bodyBuffer;
    }
}

void Request::isReqWellFormed(Request &req, long long maxBodySize) const{
    ParseRequest parse;
    (void)maxBodySize;

    parse.parseMethod(req.method);
    parse.parseHeaders(req.headers, req.method);
    parse.parseUri(req.uri);
    parse.parseVersion(req.version);
    // parse.parseBody(req.body, maxBodySize);
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

//start here
void     Request::getCheckRequest(Request &reqObj, const server &serve, int &fdSock) {
    std::string reqStr;
    // Request use
    
    reqStr = reqObj.readRequest(fdSock);
    //handling one client only 
    // reqObj.client.state = 1;
    reqObj.requestPartitioning(reqObj, reqStr);
    //done reading from socket if method is GET or DELETE 
    if (reqObj.method != "POST")
        reqObj.user.r_done = 1;
    reqObj.isReqWellFormed(reqObj, serve.getClientMaxBodySize());
    reqObj.retreiveRequestedResource(serve);
    // reqObj.setContentLength
    
    // return reqObj;
}

// void Request::locateMatchingRequestURI(const server& use) const {
//     std::vector<location> locations;
//     std::string matchedLocation;
//     size_t i;

//     locations = use.getLocations();
//     for (i = 0; i < locations.size(); i++) {
//         if (this->uri == locations[i].location_name) {
//             matchedLocation = locations[i].location_name;
//             break;
//         }
//     }
//     if (matchedLocation.empty())
//         throw std::runtime_error("404 Not Found : Matching uri location not found");
// }

int sameUntilIndex(const std::string &uri, const std::string &locationName){
    size_t i = 0;
    while (uri[i] && locationName[i] && uri[i] == locationName[i])
        i++;
    return i;
}

const location &Request::getMatchingLocation(const server &serve) {
    int counter;

    size_t i = 0;
    size_t locationsSize = serve.getLocations().size();
    counter = sameUntilIndex(getUri(), serve.getLocations().at(i).location_name);
    i++;
    for (; i < locationsSize; i++){
        if (sameUntilIndex(getUri(), serve.getLocations().at(0).location_name) > counter)
            counter = sameUntilIndex(getUri(), serve.getLocations().at(0).location_name);  
    }
    if (i >= locationsSize)
        throw std::runtime_error("404 Not found : No matching location");
    
    if (!allowedMethod(const_cast<location &>(serve.getLocations().at(i))))
        throw std::runtime_error("405 Method Not Allowed");
    
    return serve.getLocations().at(i);
}

void Request::retreiveRequestedResource(const server &serve){
    matchedLocation = getMatchingLocation(serve);
    //see the root of the location retrieved and join it with the uri then look for it using access
    //pass "/"
    std::string url = getUri().substr(1);
    fileName = url.empty() ? "index.html" : url;
    
    path = matchedLocation.location_name + fileName;
    isFileAvailable();
}

void Request::isFileAvailable(){
    
    if (stat(path.c_str(), &pathStatus) != 0)
        throw std::runtime_error("404 Not found : Requested Resource not found");
}

const server &Request::getServerInfo(void) const {
    return this->serverInfo;
}

const location &Request::getMatchedLocation(void) const {
    return this->matchedLocation;
}