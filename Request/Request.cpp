/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 15:33:33 by sel-jama          #+#    #+#             */
/*   Updated: 2024/04/29 20:12:08 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "../sock2/includes/client.hpp"
#include "../sock2/includes/server.hpp"
#include "../Response/Response.hpp"
#include "../error/errorPage.hpp"

Request::Request() : method(""), uri(""), version(""), body(""), reqStr(""), bodySaver(""),readBody(0), firstRead(1), headersDone(0), errorCode(0){
}

Request::~Request(){}

const std::string& Request::getMethod() const{
    return this->method;
}

const std::string& Request::getUri() const{
    return this->uri;
}

// const std::string& Request::getBody() const{
//     return this->body;
// }

const std::map<std::string, std::string>& Request::getHeaders() const{
    return this->headers;
}

void Request::cutOffBodySegment(std::string &request){
    size_t pos = request.find("\r\n\r\n");

    if(pos != std::string::npos){
        pos += 4;
        bodySaver = request.substr(pos);
        // int num = request.length() - pos;
        request.erase(pos, request.length() - 1);
        headersDone = 1;
    }
}

std::string Request::readRequest(int &fdSocket){
    std::stringstream buff("");
    if (readBody && firstRead){
        std::stringstream buff(bodySaver);
        firstRead = 0;
    }
        
    char buffer[BUFFER_SIZE];
    readbytes = read(fdSocket, buffer, BUFFER_SIZE - 1);
    if (readbytes < 0){
        errorCode = 500;
        throw std::runtime_error("Error reading from socket: socket failed");
    }
    else if (readbytes == 0)//flag hada sala fhadi ola -1 (same with write response -1)
        throw std::runtime_error("Peer closed the connection");
    buffer[readbytes] = '\0';
    buff << buffer;
    
    // char recvline[BUFFER_SIZE];
    // while ((readbytes = read(fdSocket, recvline, BUFFER_SIZE)) > 0){
    //     buff << recvline;
    //     if (!readBody && strstr(recvline, "\n\r\n\r"))  //change later >> break when content len is all served
    //         break;
    //     memset(recvline, 0, BUFFER_SIZE);
    // }
    // if (readbytes < 0)
    //     throw std::runtime_error("Error reading from socket: socket failed");
    std::string request(buff.str());
    if (!readBody){
        cutOffBodySegment(request);
        if (headersDone)
            readBody = 1;
    }
    //if readBody==1 the return is the body (for post)
    return request;
}

// Function to parse HTTP request
void Request::requestPartitioning(Request &saver, std::string& request) {
    std::istringstream iss(request);

    iss >> saver.method >> saver.uri >> saver.version;
    // Parse header
    std::string headerLine("");
    while (std::getline(iss, headerLine)) {
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


void Request::isReqWellFormed(Request &req, long long maxBodySize){
    ParseRequest parse;
    (void)maxBodySize;

    errorCode = parse.parseMethod(req.method);
    errorCode = parse.parseHeaders(req.headers, req.method);
    errorCode = parse.parseUri(req.uri);
    errorCode = parse.parseVersion(req.version);
    if (errorCode)
        throw std::runtime_error("error");
    // parse.parseBody(req.body, maxBodySize);
    /*if => Request body larger than client max body size in config file*/

}

bool Request::allowedMethod(location& location) const {
    std::map<std::string, int>::iterator it = location.http_methods.begin();
    for (; it != location.http_methods.end(); it++){
        if (it->first == this->method && it->second == 0)
            return false;
    }
    return true;
}

//start here
int    Request::getCheckRequest(client &client, const server &serve) {
    // std::string reqStr;
    // Request use
    try{
        client.reqq.reqStr += client.reqq.readRequest(client.ssocket);
        if (!client.reqq.headersDone)
            return 1;
        // if (client.reqq.reqStr.length() > BUFFER_SIZE)
        client.reqq.requestPartitioning(client.reqq, client.reqq.reqStr);
        // std::map<std::string, std::string>::iterator it = client.reqq.headers.begin();
        // for (; it != client.reqq.headers.end(); ++it){
        //     std::cout << it->first << " : " << it->second << std::endl;
        // }
        //done reading from socket if method is GET or DELETE 
        client.reqq.isReqWellFormed(client.reqq, serve.getClientMaxBodySize());
        // std::cout << "Request :\n"<< client.reqq.reqStr << std::endl;
        if (client.reqq.method != "POST" && client.reqq.headersDone)
            client.r_done = 1;
        client.reqq.retreiveRequestedResource(serve);
    }
    catch(const std::runtime_error &e){
        std::cout << "my code error ??????  " << client.reqq.errorCode << std::endl;
        if (client.reqq.errorCode)
        {
            client.r_done = 1;
            return 1;
            // client.reqq.response = errorPage::serveErrorPage(client.reqq);
        }
        std::cerr << e.what() << std::endl;
        throw std::runtime_error("peer closed connection");
    }
    return 1;
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

const location& Request::getMatchingLocation(const server& serve) {
    int maxCounter = -1; 
    size_t maxIndex = 0;

    for (size_t i = 0; i < serve.getLocations().size(); ++i) {
        int counter = sameUntilIndex(getUri(), serve.getLocations().at(i).location_name);
        if (counter > maxCounter) {
            maxCounter = counter;
            maxIndex = i;
        }
    }

    if (maxCounter == -1){
        errorCode = 404;
        throw std::runtime_error("404 Not Found: No matching location");
    }

    const location& matchingLocation = serve.getLocations().at(maxIndex);

    // Check if method is allowed for the matching location
    // if (!allowedMethod(const_cast<location&>(matchingLocation))){
    //     errorCode = 405;
    //     throw std::runtime_error("405 Method Not Allowed");
    // }

    return matchingLocation;
}

// const location &Request::getMatchingLocation(const server &serve) {
//     int counter;

//     size_t i = 0;
//     size_t locationsSize = serve.getLocations().size();
//     counter = sameUntilIndex(getUri(), serve.getLocations().at(i).location_name);
//     i++;
//     for (; i < locationsSize; i++){
//         if (sameUntilIndex(getUri(), serve.getLocations().at(i).location_name) > counter)
//             counter = sameUntilIndex(getUri(), serve.getLocations().at(i).location_name);  
//     }
//     if (i >= locationsSize)
//         throw std::runtime_error("404 Not found : No matching location");
    
//     if (!allowedMethod(const_cast<location &>(serve.getLocations().at(i))))
//         throw std::runtime_error("4 Method Not Allowed");
//     std::cout << "*********" << std::endl;
    
//     return serve.getLocations().at(i);
// }

void Request::retreiveRequestedResource(const server &serve){
    matchedLocation = getMatchingLocation(serve);
    //see the root of the location retrieved and join it with the uri then look for it using access
    //pass "/"
    std::string url = getUri().substr(1);
    fileName = url;
    
    path = matchedLocation.root;
    path += fileName.empty() ? "" : "/";
    path += fileName;
    std::cout <<"path : " << path << std::endl;
    isFileAvailable();
    isMethodAllowed();
}

void Request::isMethodAllowed(){
    if (!allowedMethod(const_cast<location&>(matchedLocation))){
        errorCode = 405;
        throw std::runtime_error("405 Method Not Allowed");
    }
}

void Request::isFileAvailable() {
    if (stat(path.c_str(), &pathStatus) != 0) {
    // std::cout << "debug msg " << path << std::endl;
        // if (errno == ENOENT) {
            errorCode = 404;
            throw std::runtime_error("404 Not Found: Requested Resource not found");
        // } else{
        //     // throw std::runtime_error("Error checking file availability");
        //     errorCode = 
        // }
    }
}

// void Request::isFileAvailable(){
    
//     if (stat(path.c_str(), &pathStatus) != 0)
//         throw std::runtime_error("404 Not found : Requested Resource not found");
// }

// const server &Request::getServerInfo(void) const {
//     return this->serverInfo;
// }

const location &Request::getMatchedLocation(void) const {
    return this->matchedLocation;
}

int Request::send_response(client &client){
    std::cout << "\033[1;34m started responding here \033[0m" << std::endl;
    client.w_done = 0;
    try{
        std::string content;
        if (!client.reqq.errorCode){
            content = Response::handleMethod(client);
        }
        if (client.reqq.errorCode || content.empty()){
            std::cout << "got here "<< std::endl;
            content = errorPage::serveErrorPage(client.reqq);
        }
        
        std::string res;
        std::ostringstream response;
        errorPage msg;
        if (!client.reqq.errorCode)
            client.reqq.errorCode = 200;
        response << "HTTP/1.1 " << client.reqq.errorCode << " " << msg.errorMsgs[client.reqq.errorCode] << "\r\n"
            //  << "Content-Type: " << mimeType << "\r\n"
             << "Content-Length: " << content.length() << "\r\n"
             << "\r\n";
        std::cout << "\033[1;35m---------------RESPONSE-----------------\n" <<  response.str() <<"\033[0m" << std::endl;
        response << content;
        res = response.str();
        // std::cout << "Response: \n"<<res;
        // write(client.ssocket, res.c_str(), res.length());
        if (send(client.ssocket, res.c_str(), res.length(), 0) == -1){
            std::cout << "Error: Failed to send response to client"<< std::endl;
            return 0;
        }
        
        client.w_done = 1;
        if (client.w_done){
            client.reqq.method = "";
            client.reqq.uri = "";
            client.reqq.version = "";
            client.reqq.body = "";
            client.reqq.reqStr = "";
            client.reqq.bodySaver = "";
            client.reqq.readBody = 0;
            client.reqq.firstRead = 1;
            client.reqq.headersDone = 0;
            client.reqq.errorCode = 0;
        }
    }
    catch (const std::runtime_error &e){
        std::cout << "Exception catched in send response : " << e.what() << std::endl;
        return 0;
    }
    return 1;
}

int Request::read_request(client &client, server &server){
    client.r_done = 0;
    try{
        if (!readBody){
            // std::cout << "\033[1;31m reading HEADERS here \033[0m" << std::endl;
            getCheckRequest(client, server);
        }
        else{
            // std::cout << "\033[1;33m reading BODY here \033[0m" << std::endl;
            Post::body(client);
        }
    }
    catch (const std::runtime_error &e){
        std::cout << "\033[1;36mError in reading : "<< e.what() << "\033[0m" << std::endl;
        if(!errorCode)
            return 0;
    }
    std::cout << "\033[1;33m--------------------REQUEST-------------------------\n" << client.reqq.reqStr << client.reqq.body << "\033[0m" << std::endl;
    return 1;
}