/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 15:33:33 by sel-jama          #+#    #+#             */
/*   Updated: 2024/05/08 04:27:18 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "../sock2/includes/client.hpp"
#include "../sock2/includes/server.hpp"
#include "../Response/Response.hpp"
#include "../error/errorPage.hpp"
#include "../sock2/includes/infra.hpp"
#include <time.h>
Request::Request() : method(""), uri(""), 
version(""), body(""), reqStr(""), responseContentType(""), responseContentLen(0), fileName(""),
contentLength(0), readbytes(0), readBody(0), firstRead(1)
,headersDone(0), statusCode(0), statusMsg(""), isChunked(0), 
cgi(0), chunkPos(0), firstChunk(1), locationHeader(""), 
cgi_File(""), cgi_File2("") , responseDone(0) ,filePath(""), filePosition(0), responseHeaders(""),r(0){
    size_body = 0; to_de = 0 ;flag = 0; saver_count = 0; tmp = 0; chunked_flag = 0; flag2 = 0; filename__ = "";
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

const std::string& Request::getVersion() const{
    return this->version;
}

void Request::load_extension()
{
    std::ifstream file("MIME.conf");
    std::string buffer;
    std::string secbuffer;
    std::string forvalue;
    std::string forkey;
    if (file.is_open()) {
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
        file.close();
    }
}

void Request::cutOffBodySegment(std::string &request){
    size_t pos = request.find("\r\n\r\n");

    if(pos != std::string::npos){
        pos += 4;
        body = request.substr(pos);
        request.erase(pos, request.length() - 1);
        headersDone = 1;
    }
}

std::string Request::readRequest(client &client){
    std::stringstream buff("");
    char buffer[BUFFER_SIZE];
    readbytes = read(client.ssocket, buffer, BUFFER_SIZE - 1);
    client.wakt = time(NULL);
    if (readbytes < 0){
        statusCode = 500;
        throw std::runtime_error("Error reading from socket");
    }
    else if (readbytes == 0){
        statusCode = -1;
        throw std::runtime_error("Peer closed the connection");
    }
    else
        recentAction = time(NULL);

    buffer[readbytes] = '\0';
    buff.write(buffer, readbytes);
    std::string request(buff.str());
   
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

void trim(std::string &str){
    while (!str.empty() && std::isspace(str[0]))
        str.erase(0, 1);

    while (!str.empty() && std::isspace(str[str.length() - 1]))
        str.erase(str.length() - 1);
}

void Request::checkRequestLine(Request &saver, std::string &line){
    int err = 0;
    int count = 0;
    for (size_t i = 0; i < line.size(); i++){
        if (std::isspace((int)line[i]) && line[i] != ' ' && i != line.size()-1)
            err = 1;
        else if(line[i] == ' '){
            count++;
            if (i == 0 || i == line.size()-1)
                err = 1;
            else if (count >= 3)
                err = 1;
        }
    }
    if (err){
        statusCode = 400;
        throw std::runtime_error("bad request");
    }
    std::istringstream ss(line);
    ss >> saver.method >> saver.uri >> saver.version;
    
    if (saver.method.empty() || saver.uri.empty() || saver.version.empty())
    {
        statusCode = 400;
        throw std::runtime_error("bad request line");
    }
}


void lowerHeader(std::string& key){
    for(size_t i=0; i<key.size(); i++){
        key[i] = tolower(key[i]);
    }
}

// Function to parse HTTP request
void Request::requestPartitioning(Request &saver, std::string& request) {
    std::stringstream iss(request);
    std::string headerLine("");
    std::getline(iss, headerLine, '\n');

    checkRequestLine(saver, headerLine);
    uriQuery(saver.uri);
    // Parse header
    while (std::getline(iss, headerLine, '\n')){
        if (headerLine == "\r")
            break;
        if (headerLine.length() > 1024)
        {
            statusCode = 413;
            throw std::runtime_error("Too long headerline");
        }
        size_t pos = headerLine.find(':');
        if (pos != std::string::npos){
            std::string key = headerLine.substr(0, pos);
            std::string value = headerLine.substr(pos + 1);
            trim(key);
            trim(value);
            if (key.empty() || value.empty()){
                saver.statusCode = 400;
                throw std::runtime_error("bad headers format");
            }
            lowerHeader(key);
            saver.headers[key] = value;
        }
        else {
            saver.statusCode = 400;
            throw std::runtime_error("bad headers");
        }
    }

    std::map<std::string, std::string>::const_iterator it = saver.headers.find("content-length");
    if (it != saver.headers.end()) {
        std::stringstream ss(it->second);
        std::cout << it->second << std::endl;
        double len;
        char *ptr;
        len = strtod(it->second.c_str(), &ptr);
        if (*ptr != '\0'){
            saver.statusCode = 400;
            throw std::runtime_error("bad request");
        }
        if (it->second.empty() || it->second.at(0) == '-' || !(ss >> contentLength)){
            saver.statusCode = 400;
            throw std::runtime_error("bad request");
        }
        std::cout << contentLength <<  std::endl;
        if (contentLength < 0){
            saver.statusCode = 400;
            throw std::runtime_error("bad request");
        }
    }
    
    it = saver.headers.find("transfer-encoding");
    if (it != saver.headers.end() && headers["transfer-encoding"] == "chunked")
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
        client.reqq.reqStr.append(client.reqq.readRequest(client));
        if (!client.reqq.readBody)
            client.reqq.cutOffBodySegment(client.reqq.reqStr);
    
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
        server serve = client.reqq.getMatchedServer(client.port_server, client.adress_server, infra);
        client.reqq.errorPages = serve.errorPages;
        client.reqq.retreiveRequestedResource(serve);
        if (!client.reqq.readBody && client.reqq.headersDone && client.reqq.method == "POST"){
                client.reqq.readBody = 1;
        }
    return 1;
}

int sameUntilIndex(const std::string &uri, const std::string &locationName){
    size_t i = 0;
    while (uri[i] && locationName[i] && uri[i] == locationName[i])
        i++;

    if (!uri[i] && locationName[i])
        i = -1;
    return i;
}

const location& Request::getMatchingLocation(const server& serve){
    int maxCounter = -1; 
    size_t maxIndex = 0;

    for (size_t i = 0; i < serve.getLocations().size(); ++i){
        int counter = sameUntilIndex(getUri(), serve.getLocations().at(i).location_name);
        if (counter > maxCounter){
            maxCounter = counter;
            maxIndex = i;
        }
    }

    const location& matchingLocation = serve.getLocations().at(maxIndex);
    return matchingLocation;
}

void Request::retreiveRequestedResource(const server &serve){
    matchedLocation = getMatchingLocation(serve);
    fileName = getUri();
    
    path = matchedLocation.root;
    path += fileName.substr(matchedLocation.location_name.length());
    // std::cout <<"path : " << path << std::endl;
    isMethodAllowed();
    isFileAvailable();
    isRedirect();
}

void Request::isMethodAllowed(){
    if (!allowedMethod(const_cast<location&>(matchedLocation))){
        statusCode = 405;
        throw std::runtime_error("405 Method Not Allowed");
    }
}

void Request::isFileAvailable() {
    if (stat(path.c_str(), &pathStatus) != 0) {
            statusCode = 404;
            throw std::runtime_error("404 Not Found: Requested Resource not found");
    }
}

void Request::isRedirect() {
    if (!matchedLocation.retu.empty()){
        locationHeader = matchedLocation.retu;
        statusCode = 301;
        throw std::runtime_error("Redirect");
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
    req.queryString = "";
    req.response = "";
    req.chunkPos = 0;
    req.firstChunk = 1;
    req.locationHeader = "";
    req.responseContentType = "";
    req.responseContentLen = 0;
    req.cgi_File = "";
    req.cgi_File2 = "";
    req.filePath = "";
    req.responseDone = 0;
    req.responseHeaders = "";
    req.flag2 = 0;
    req.filename__ = "";
    req.size_body = 0;
    req.getit = "";
    req.r_s = "";
    req.flag = 0;
    req.filname_s = "";
    req.saver_count = 0;
    req.tmp = 0;
    req.to_de = 0;
    req.to_de2 = 0;
    req.content_T = "";
    req.path = "";
    req.fileName = "";
    req.chunked_flag = 0;
    req.chunkPos = 0;

}

std::string Request::generateResponse(client &client, std::string &content){
    client.reqq.responseContentType = getMimeType(client.reqq.fileName);
    if (!responseContentLen){
            responseContentLen = content.length();
    }
    if ((client.reqq.method == "POST" || client.reqq.method == "DELETE") && client.reqq.statusCode < 300){
        responseContentLen = 0;
        content = "";
    }
    std::string res;
    std::stringstream response;
    errorPage msg;
    // if (!client.reqq.statusCode){
    //     if (client.reqq.method == "GET")
    //         client.reqq.statusCode = 200;
    //     else
    //         responseContentLen = 500;
    // }
    response << "HTTP/1.1 " << client.reqq.statusCode << " " << msg.statusMsgs[client.reqq.statusCode] << "\r\n";
    if (!client.reqq.cgi || client.reqq.statusCode >= 300){
        response << "Content-Type: " << responseContentType << "\r\n";
        if (!client.reqq.locationHeader.empty()){
            response << "Location: " << client.reqq.locationHeader << "\r\n";
            response << "Content-Length: 0\r\n\r\n";
            return response.str();
        }
        response << "Content-Length: " << responseContentLen << "\r\n\r\n";
        // if (client.reqq.headers.find("Cookie") != client.reqq.headers.end())
        //     response << "Cookie: " << client.reqq.headers["Cookie"];
    }

    response << content;
    res = response.str();
    return res;
}

std::string Request::getChunk(Request &req){
        std::ifstream file(req.filePath.c_str());
        if (!file.is_open()) {
            req.statusCode = 500;
            throw std::runtime_error("Failed to read requested content");
        }

        // Seek to the position where we left off last time
        file.seekg(filePosition);

        char buffer[1024];

        std::ostringstream content;
        std::streamsize bytesRead = file.readsome(buffer, 1024);
        
        if (bytesRead > 0) {
            content.write(buffer, bytesRead);
            filePosition = file.tellg(); // Update file pos
        }
        else
            return "";

        file.close();
        return content.str();
}

void Request::checkSatusCode(Request &req){
    if (!req.statusCode){
        if (req.method == "GET")
            req.statusCode = 200;
        else
            req.statusCode = 500;
    }
}

int Request::send_response(client &client){
    std::string chunk("");
    
    if (firstChunk){
        client.w_done = 0;
        std::string content;
        try{
            if (!client.reqq.cgi){
                checkSatusCode(client.reqq);
                if (client.reqq.statusCode >= 300)
                    throw std::runtime_error("Request reading failed");
                content = Response::handleMethod(client);
            }
            if (content.empty() && client.reqq.cgi){
                client.reqq.get.cgi.checkTimeout(client.reqq);
                if (!client.reqq.get.cgi.response.empty())
                    content = client.reqq.get.cgi.response;
                else{
                    client.w_done = 0;
                    return 1;
                }
            }
        }
        catch (const std::runtime_error &e){
            if (client.reqq.method != "HEAD"){
                std::cout << e.what() << std::endl;
                content = errorPage::serveErrorPage(client.reqq);
                responseContentLen = content.length();
            }
        }
        response = generateResponse(client, content);
        responseHeaders = response;
        firstChunk = 0;
    }
    else if (client.reqq.method == "GET" && !client.reqq.filePath.empty()){
        chunkPos = 0;
        response = getChunk(client.reqq);
        if (response.empty())
            responseDone = 1;
    }
    
    if (chunkPos < response.length() - 1){
        if (responseDone){
            resetClientRequest(client.reqq);
            client.w_done = 1;
            client.wakt = time(NULL);
            return 1;
        }
        if (client.reqq.filePath.empty())
            chunk = response.substr(chunkPos, 1024);
        else
            chunk = response;
        int sret = send(client.ssocket, chunk.c_str(), chunk.length(), 0);
        if (sret == -1){
            std::cerr << "send failed ..." << std::endl;
            return 0;
        }
        if (sret == 0){
            std::cerr << "Peer closed connection ... " << std::endl;
            return 0;
        }
        chunkPos += 1024;
    }
    else{
        resetClientRequest(client.reqq);
        client.w_done = 1;
    }
    client.wakt = time(NULL);
    return 1;
}

int Request::read_request(client &client, infra & infra){
    client.r_done = 0;
    try{
        if (!readBody){
            getCheckRequest(client, infra);
            if(client.reqq.method == "POST" && !client.reqq.isChunked){
                if(client.reqq.flag2 == 0)
                    Post::support_upload(client.reqq);
                if(!client.reqq.body.empty())
                {
                    if (client.reqq.contentLength < client.reqq.body.size()){
                        client.reqq.file.write(client.reqq.body.c_str(), client.reqq.contentLength);
                        client.reqq.size_body += client.reqq.contentLength;
                    }
                    else{
                        client.reqq.file.write(client.reqq.body.c_str(), client.reqq.body.size());
                        client.reqq.size_body += client.reqq.body.size();
                    }
                    if(client.reqq.body.size() >= client.reqq.contentLength){
                        client.reqq.file.close();
                        client.reqq.statusCode = 201;
                        client.reqq.responseContentLen = client.reqq.body.length();
                        client.r_done = 1;
                    }
                    client.reqq.body = "";
                }
            }
        }
        else if(readBody){
            if(!client.reqq.isChunked)
            {
                Post::body(client);
            }
            else{
                Post::chunked_body2(client);
                if(client.reqq.chunked_flag == 0)
                    Post::chunked_body(client);
            }
        }
    }
    catch (const std::runtime_error &e){
        client.r_done = 1;
        if(statusCode == -1){
            return 0;
        }
    }
    return 1;
}

void lowerS(std::string& key){
    for(size_t i=0; i<key.size(); i++){
        key[i] = tolower(key[i]);
    }
}

const server &Request::getMatchedServer(uint16_t &listenport, std::string &listenaddr, const infra &infra){
    std::string reqServerName;
     std::map<std::string, std::string> copy;
    for(std::map<std::string, std::string>::iterator i = headers.begin(); i != headers.end(); ++i){
        std::string key = i->first;
        std::string value = i->second;
        lowerS(key);
        copy[key] = value;
    }

    std::map<std::string, std::string>::const_iterator it = copy.find("host");
    if (it != copy.end()) {
        reqServerName = it->second;
    }
    int found = 0;
    std::vector<server>::const_iterator save;
    
    std::vector<server>::const_iterator i = infra.getServer().begin();
    save = i;
    for (; i!=infra.getServer().end(); ++i){
        if (i->port == listenport && ((i->adress == listenaddr) || (i->adress == "127.0.0.1" && listenaddr == "localhost") 
            || (i->adress == "localhost" && listenaddr == "127.0.0.1")) ){
                if(!found){
                    save = i;
                    found = 1;
                }
                for (size_t s = 0; s < i->serverName.size(); ++s){
                    if (i->serverName[s] == reqServerName)
                        return *i;
                }
            }
    }
    return *(save);
}

std::string Request::getMimeType(const std::string& fileName){
    load_extension();
    size_t dotPos = fileName.find_last_of('.');
    if (dotPos != std::string::npos){
        std::string exe = fileName.substr(dotPos);
        std::map<std::string, std::string>::iterator it = extension.begin();
        for (; it != extension.end(); it++){
            if (it->second == exe)
                return it->first;
        }
    }
    return "text/html";
}

bool Request::checkReadingTimout(const client &client){
    if (!headersDone && time(NULL) - recentAction >= REQUEST_TIMEOUT)
        return true;
    if (readBody && !client.r_done && time(NULL) - recentAction >= REQUEST_TIMEOUT)
        return true;
    return false;

}