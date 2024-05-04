/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 15:33:33 by sel-jama          #+#    #+#             */
/*   Updated: 2024/05/04 01:02:10 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "../sock2/includes/client.hpp"
#include "../sock2/includes/server.hpp"
#include "../Response/Response.hpp"
#include "../error/errorPage.hpp"
#include "../sock2/includes/infra.hpp"
Request::Request() : method(""), uri(""), 
version(""), body(""), reqStr(""), responseContentType(""), responseContentLen(0), fileName(""),
contentLength(0), readbytes(0), readBody(0), firstRead(1)
,headersDone(0), statusCode(0), statusMsg(""), isChunked(0), cgi(0), r(0){
    to_de = 0 ;flag = 0; saver_count = 0; tmp = 0;
}

Request::~Request(){}

const std::string& Request::getMethod() const{
    return this->method;
}

const std::string& Request::getUri() const{
    return this->uri;
}

const std::map<std::string, std::string>& Request::getHeaders() const{
    return this->headers;
}

const std::string& Request::getQuryString() const{
    return this->queryString;
}

void Request::load_extension()
{
    std::ifstream file("/goinfre/yboucha/cloned/Request/MIME.conf");
    std::string buffer;
    std::string secbuffer;
    std::string forvalue;
    std::string forkey;
    if (file.is_open()) {
        while(std::getline(file, buffer))
        {
                std::stringstream ss(buffer);
                // std::cout << buffer << std::endl;
                for (int i = 0; getline(ss, secbuffer, ' '); i++) {

                            // std::cout << secbuffer << "."<< std::endl;
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

void Request::cutOffBodySegment(std::string &request){
    size_t pos = request.find("\r\n\r\n");

    if(pos != std::string::npos){
        pos += 4;
        body = request.substr(pos);
        // int num = request.length() - pos;
        request.erase(pos, request.length() - 1);
        headersDone = 1;
    }
}

std::string Request::readRequest(int &fdSocket){
    std::stringstream buff("");
    // if (readBody && firstRead){
    //     buff << bodySaver; 
    //     firstRead = 0;
    // }
    
    
    char buffer[BUFFER_SIZE];
    // std::cout << "readbytes-> " << readbytes << std::endl;
    readbytes = read(fdSocket, buffer, BUFFER_SIZE - 1);
    // std::cout << "readbytes: " << readbytes << std::endl;
    if (readbytes <= 0){
        statusCode = 500;
        throw std::runtime_error("Error reading from socket: Peer closed connection");
    }
    // else if (readbytes == 0)//flag hada sala fhadi ola -1 (same with write response -1)
    //     throw std::runtime_error("Peer closed the connection");
    buffer[readbytes] = '\0';
    buff.write(buffer, readbytes);
    std::string request(buff.str());
    if (!readBody){
        cutOffBodySegment(request);
        if (headersDone)
            readBody = 1;
            
    }
    return request;
}

void Request::uriQuery(std::string &uri){
    size_t pos = uri.find('?');
    
    if (pos != std::string::npos){
        this->queryString = uri.substr(pos+1);
        uri = uri.substr(0, pos);
    }
    else
        this->queryString = "";
}

// Function to parse HTTP request
void Request::requestPartitioning(Request &saver, std::string& request) {
    std::stringstream iss(request);

    iss >> saver.method >> saver.uri >> saver.version;
    uriQuery(saver.uri);
    // Parse header
    std::string headerLine("");
    std::getline(iss, headerLine, '\n');
    while (std::getline(iss, headerLine, '\n')){
        if (headerLine == "\r")
            break;
        size_t pos = headerLine.find(':');
        if (pos != std::string::npos) {
            std::string key = headerLine.substr(0, pos);
            std::string value = headerLine.substr(pos + 1);
            while (!key.empty() && isspace(key.front())) key.erase(0, 1);
            while (!key.empty() && isspace(key.back())) key.pop_back();
            while (!value.empty() && isspace(value.front())) value.erase(0, 1);
            while (!value.empty() && isspace(value.back())) value.pop_back();
            if (key.empty() || value.empty()){
                saver.statusCode = 400;
                throw std::runtime_error("bad request");
            }
            saver.headers[key] = value;
        }
        else {
            saver.statusCode = 400;
            throw std::runtime_error("bad request");
        }
    }

    std::map<std::string, std::string>::const_iterator it = saver.headers.find("Content-Length");
    if (it != saver.headers.end()) {
        contentLength = 0;
         char *endptr;
        //  std::stringstream ss(it->second);
        //  contentLength 
        contentLength = strtod(it->second.c_str(), &endptr);
    }
    
    it = saver.headers.find("Transfer-Encoding");
    if (it != saver.headers.end() && headers["Transfer-Encoding"] == "chunked")
        isChunked = 1;
}

void Request::isReqWellFormed(Request &req){
    ParseRequest parse;
    
    statusCode = parse.parseMethod(req.method);
    if (statusCode)
        throw std::runtime_error("parse error");
    statusCode = parse.parseHeaders(req.headers, req.method);
    if (statusCode)
        throw std::runtime_error("parse error");
    statusCode = parse.parseUri(req.uri);
    if (statusCode)
        throw std::runtime_error("parse error");
    statusCode = parse.parseVersion(req.version);
    if (statusCode)
        throw std::runtime_error("parse error");
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
int    Request::getCheckRequest(client &client, const infra &infra) {
        client.reqq.reqStr.append(client.reqq.readRequest(client.ssocket));
        if (!client.reqq.headersDone)
            return 1;
        
        client.reqq.requestPartitioning(client.reqq, client.reqq.reqStr);
        client.reqq.isReqWellFormed(client.reqq);
        
        //done reading from socket if method is GET or DELETE 
        if (client.reqq.method != "POST" && client.reqq.headersDone)
            client.r_done = 1;
        
       if(!client.reqq.isChunked && (static_cast<size_t>(client.reqq.contentLength) <= client.reqq.body.size()))
        {
            client.reqq.readBody = 0;
            client.r_done = 1;
        }
        server serve = client.reqq.getMatchedServer(infra);
        // client.reqq.errorPages = serve.errorPages;
        client.reqq.retreiveRequestedResource(serve);
    return 1;
}

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
        statusCode = 404;
        throw std::runtime_error("404 Not Found: No matching location");
    }

    const location& matchingLocation = serve.getLocations().at(maxIndex);
    return matchingLocation;
}

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
        statusCode = 405;
        throw std::runtime_error("405 Method Not Allowed");
    }
}

void Request::isFileAvailable() {
    if (stat(path.c_str(), &pathStatus) != 0) {
        if (errno == ENOENT) {
            statusCode = 404;
            throw std::runtime_error("404 Not Found: Requested Resource not found");
        } else{
            statusCode = 500;
            throw std::runtime_error("Error checking file availability");
        }
    }
}

const location &Request::getMatchedLocation(void) const {
    return this->matchedLocation;
}

void resetClientRequest(Request &req){
    req.method = "";
    req.uri = "";
    req.version = "";
    req.body = "";
    req.reqStr = "";
    req.readBody = 0;
    req.firstRead = 1;
    req.headersDone = 0;
    req.statusCode = 0;
    req.isChunked = 0;
    req.serverd = 0;
    req.cgi = 0;
}

std::string Request::generateResponse(client &client, std::string &content){
    if (!responseContentLen)
            responseContentLen = content.length();
        std::string res;
        std::stringstream response;
        errorPage msg;
        if (!client.reqq.statusCode)
            client.reqq.statusCode = 200;
        response << "HTTP/1.1 " << client.reqq.statusCode << " " << msg.statusMsgs[client.reqq.statusCode] << "\r\n";
        if (!client.reqq.cgi){
            response << "Content-Type: " << responseContentType << "\r\n"
            << "Content-Length: " << responseContentLen << "\r\n"
            << "\r\n";
        }
        
        response << content;
        res = response.str();
        return res;
}

int Request::send_response(client &client){
    // std::cout << "\033[1;34m started responding here \033[0m" << std::endl;
    client.w_done = 0;
    std::string res;
    try{

        std::string content;
        // std::cout << client.reqq.statusCode << "*********----------"<< std::endl;
        if (!client.reqq.statusCode){
            content = Response::handleMethod(client);
        }
        //post do not give content handle later  <<<<<<<
        if (client.reqq.statusCode || content.empty()){
            // std::cout << "got here "<< std::endl;
            if(client.reqq.method != "HEAD")
                content = errorPage::serveErrorPage(client.reqq);
        }
        // if (!client.reqq.cgi)
            res = generateResponse(client, content);
        // else
            // res = content;
        std::cout << "\033[1;35m---------------RESPONSE-----------------\n" <<  res <<"\033[0m" << std::endl;
        // char buffer[BUFFER_SIZE] = {};
        // response.read(buffer, BUFFER_SIZE);
        // buffer[response.gcount()] = '\0';
        // std::streamsize bytes = response.readsome(buffer, BUFFER_SIZE);
        
        // std::cout << "Response: \n"<<res;
        // write(client.ssocket, res.c_str(), res.length());
        // if (response.eof()){
        client.w_done = 1;
        resetClientRequest(client.reqq);
        // }
        if (send(client.ssocket, res.c_str(), res.length(), 0) == -1)
            throw std::runtime_error("Send failed");
    }
    catch (const std::runtime_error &e){
        std::cout << "Exception catched in send response : " << e.what() << std::endl;
        return 0;
    }
    return 1;
}

int Request::read_request(client &client, infra & infra){
    client.r_done = 0;
    try{
        if (!readBody){
            // std::cout << "\033[1;31m reading HEADERS here \033[0m" << std::endl;
            // client.reqq.errorPages = server.errorPages;
            getCheckRequest(client, infra);
        }
        else{
            if(!client.reqq.isChunked)
                Post::body(client);
            else {
                // std::cout << "Enter" << std::endl;
                // throw std::runtime_error("eror");
                Post::chunked_body2(client);
                if(client.reqq.chunked_flag == 0)
                    Post::chunked_body(client);
            }
        }
        }
    catch (const std::runtime_error &e){
        std::cout << "\033[1;36mError in reading : "<< e.what() << "\033[0m" << std::endl;
        client.r_done = 1;
        // if(!statusCode)
            // return 0;
    }
    if (client.r_done)
        std::cout << "\033[1;33m--------------------REQUEST-------------------------\n" << client.reqq.reqStr << "\033[0m" << std::endl;

    return 1;
}

const server &Request::getMatchedServer(const infra &infra){
    std::map<std::string, std::string>::const_iterator it = headers.find("Host");
    if (it != headers.end()) {
        std::stringstream ss(it->second);
        std::string addr, p;
        getline(ss, addr, ':');
        getline(ss, p);
        char *endp;
        this->port = strtol(p.c_str(), &endp, 10);
        this->ip = addr;
    }
    else
        return infra.getServer().at(0);
    
    std::vector<server>::const_iterator i = infra.getServer().begin();
    for (; i!=infra.getServer().end(); ++i){
        if (i->port == this->port && ((i->adress == this->ip) || (i->adress == "127.0.0.1" && this->ip == "localhost"))) //&& servername too
            return *i;
    }
    return *i;
    
}