/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:13:04 by sel-jama          #+#    #+#             */
/*   Updated: 2024/04/25 10:02:39 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseRequest.hpp"

ParseRequest::ParseRequest() {
    setMethods();
    setUriAllowedChars();
    setHttpHeaders();
}

void ParseRequest::setHttpHeaders() {
    ngxHttpHeaders.push_back("Host");  //A domain name or IP address representing the target host
    ngxHttpHeaders.push_back("Connection"); //Controls whether the network connection should be kept open for further requests
    ngxHttpHeaders.push_back("If-Modified-Since"); //A date and time used in conditional GET requests to specify when a resource was last modified
    ngxHttpHeaders.push_back("If-Match");  //Specifies a condition for successful resource retrieval based on entity tags
    ngxHttpHeaders.push_back("If-None-Match"); //Specifies a condition for successful resource retrieval based on entity tags.
    ngxHttpHeaders.push_back("User-Agent"); //Identifies the client making the request, typically the browser or user agent software
    ngxHttpHeaders.push_back("Referer"); //The URL of the web page that linked to the resource being requested
    ngxHttpHeaders.push_back("Content-Length"); //The size of the message body in bytes
    ngxHttpHeaders.push_back("Content-Range");  //Specifies the range of bytes in the message body being sent or requested.
    ngxHttpHeaders.push_back("Content-Type"); //Specifies the media type of the message body.
    ngxHttpHeaders.push_back("Range");  //Specifies the range of bytes being requested in a GET request
    ngxHttpHeaders.push_back("If-Range"); //Specifies conditions for successful resource retrieval based on entity tags or modification dates
    ngxHttpHeaders.push_back("Transfer-Encoding");  //Specifies the encoding mechanisms used to transfer the message body
    ngxHttpHeaders.push_back("TE");  //Specifies the transfer codings that are acceptable in the response
    ngxHttpHeaders.push_back("Expect"); //Specifies expectations that need to be met by the server
    ngxHttpHeaders.push_back("Upgrade");  //Specifies additional communication protocols that the client supports
    
    ngxHttpHeaders.push_back("Allow");
    ngxHttpHeaders.push_back("Auth-Scheme");
    ngxHttpHeaders.push_back("Content-Language");
    ngxHttpHeaders.push_back("Content-Length");
    ngxHttpHeaders.push_back("Content-Location");
    ngxHttpHeaders.push_back("Last-Modified");
    ngxHttpHeaders.push_back("Location");
    ngxHttpHeaders.push_back("Retry-After");
    ngxHttpHeaders.push_back("Server");
    ngxHttpHeaders.push_back("Www-Authenticate");
  
    //HTTP_GZIP || HTTP_HEADeRS
    ngxHttpHeaders.push_back("Accept-Encoding");  //Specifies the compression algorithms supported by the client
    ngxHttpHeaders.push_back("Via");   //Specifies intermediate proxies or gateways through which the request was forwarded
    
    ngxHttpHeaders.push_back("Authorization");  //Contains credentials for authenticating the client with the server.
    ngxHttpHeaders.push_back("Keep-Alive");  //Controls whether the connection should be kept alive after the current request/response cycle
    ngxHttpHeaders.push_back("X-Forwarded-For");   //Contains the client's IP address when requests are forwarded through proxies or load balancers
    //HTTP_REALIP
    ngxHttpHeaders.push_back("X-Real-IP");    //Contains the client's real IP address when requests are forwarded through proxies or load balancers.
    //http headers
    ngxHttpHeaders.push_back("Accept");   //Specifies the media types that are acceptable for the response
    ngxHttpHeaders.push_back("Accept-Language");   //Specifies the natural languages that are preferred for the response.
    //http dav
    ngxHttpHeaders.push_back("Depth");   //Specifies the depth of a WebDAV operation.
    ngxHttpHeaders.push_back("Destination");  //Specifies the destination URL for a WebDAV operation
    ngxHttpHeaders.push_back("Date");  //Specifies the date and time at which the message was originated
    ngxHttpHeaders.push_back("Cookie");  //Specifies the date and time at which the message was originated
    
}

void ParseRequest::setMethods(void) {
    this->methods.push_back("PUT");
    this->methods.push_back("PATCH");
    this->methods.push_back("HEAD");
    this->methods.push_back("OPTIONS");
    this->methods.push_back("TRACE");
    this->methods.push_back("CONNECT");
}

void ParseRequest::setUriAllowedChars(void) {
    this->uriAllowedChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ._~:/?#[]@!$&'()*+,;=%";
}

bool ParseRequest::checkMethod(const std::string &method) const {
    if (method == "GET" || method == "POST" || method == "DELETE")
        return "true";
    return "false";
}

bool ParseRequest::unsupportedMethod(const std::string &method) const{
    for (size_t i = 0; i < this->methods.size(); i++){
        if (this->methods[i] == method)
            return true;
    }
    return false;
}

bool ParseRequest::charOccured(char c) const{
    for(size_t i = 0; i < uriAllowedChars.size(); i++){
        if (uriAllowedChars[i] == c)
            return true;
    }
    return false;
}

bool ParseRequest::checkUriAllowedChars(std::string &uri) const{
    for (size_t i = 0; i < uri.size(); i++){
        if (charOccured(uri[i]) == false)
            return false;
    }
    return true;
}

void ParseRequest::parseMethod(std::string &method) const{
    if (unsupportedMethod(method))
        throw std::runtime_error("501 Not Implemented : Method not supported");
    
    if (checkMethod(method) == false)
        throw std::runtime_error("404 Not found : Method not found");
}

void ParseRequest::parseHeaders(std::map<std::string, std::string> &headers, std::string &method) const{
    checkUnknownHeader(headers);
    
    if (headers.find("Transfer-Encoding") != headers.end()
        && headers["Transfer-Encoding"] != "chunked")
            throw std::runtime_error("501 Not Implemented : Transfer-Encoding must be chuncked");
    else if ((headers.find("Content-Length") == headers.end() || atoi(headers.at("Content-Length").c_str())) && method == "POST")
        throw std::runtime_error("400 Bad Request : Messing headers");
    
    //setContentLength(headers);

}

void ParseRequest::parseUri(std::string &uri) const {
    /*if => Request uri contain a character not allowded => bad request 400*/
    if ((uri.size() >= 1 && uri[0] != '/') || checkUriAllowedChars(uri) == false)
        throw std::runtime_error("400 Bad Request : bad URI");

    if (uri.size() > MAX_SIZE)
        throw std::runtime_error("414 Request-URI Too Long");
}

void ParseRequest::parseVersion(std::string &version) const{
    std::stringstream ss(version);
    
    std::string http;
    std::string vers;
    std::getline(ss, http, '/');
    std::getline(ss, vers);

    char *endptr;
    double v = strtod(vers.c_str(), &endptr);
  
    if (http != "HTTP" || *endptr != '\0')
        throw std::runtime_error("400 Bad Request : Error occured in Http version");
    
    if (v == 0.9 || v == 2.0 || v == 1.0 || v == 3.0)
        throw std::runtime_error("501 Not Implemented : Error occured in http version");
    
    if (v != 1.1)
        throw std::runtime_error("400 Bad Request : Error occured in Http version");
}

void ParseRequest::parseBody(std::string &body, long long &maxBodySize) const{
    if (static_cast<long long>(body.length()) > maxBodySize)
        throw std::runtime_error("413 Request Entity Too Large");
}

const std::deque<std::string> &ParseRequest::getHttpHeaders() const { return this->ngxHttpHeaders; }

void ParseRequest::checkUnknownHeader(std::map<std::string, std::string> &headers) const{
    std::map<std::string, std::string>::iterator it = headers.begin();
    for (; it != headers.end(); it++) {
        // if(ngxHttpHeaders.find(it->first) == ngxHttpHeaders.end())
        //     throw std::runtime_error("400 bad Request : unknown headers");
    }
}