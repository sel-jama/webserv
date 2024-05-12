/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:13:04 by sel-jama          #+#    #+#             */
/*   Updated: 2024/05/06 22:04:23 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseRequest.hpp"
#include <algorithm>

ParseRequest::ParseRequest() {
    // setMethods();
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
    ngxHttpHeaders.push_back("content-length"); //The size of the message body in bytes
    ngxHttpHeaders.push_back("Content-Range");  //Specifies the range of bytes in the message body being sent or requested.
    ngxHttpHeaders.push_back("content-type"); //Specifies the media type of the message body.
    ngxHttpHeaders.push_back("Range");  //Specifies the range of bytes being requested in a GET request
    ngxHttpHeaders.push_back("If-Range"); //Specifies conditions for successful resource retrieval based on entity tags or modification dates
    ngxHttpHeaders.push_back("transfer-encoding");  //Specifies the encoding mechanisms used to transfer the message body
    ngxHttpHeaders.push_back("TE");  //Specifies the transfer codings that are acceptable in the response
    ngxHttpHeaders.push_back("Expect"); //Specifies expectations that need to be met by the server
    ngxHttpHeaders.push_back("Upgrade");  //Specifies additional communication protocols that the client supports
    
    ngxHttpHeaders.push_back("Allow");
    ngxHttpHeaders.push_back("Auth-Scheme");
    ngxHttpHeaders.push_back("Content-Language");
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

void ParseRequest::setUriAllowedChars(void) {
    this->uriAllowedChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ._~:/?#[]@!$&'()*+,;=%";
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

int ParseRequest::parseMethod(std::string &method) const{
    if (method != "GET" && method != "POST" && method != "DELETE")
        return 501;
    return 0;
}

void ParseRequest::lowerString(std::string& key){
    for(size_t i=0; i<key.size(); i++){
        key[i] = tolower(key[i]);
    }
}

int ParseRequest::parseHeaders(std::map<std::string, std::string> &headers, std::string &method){
    std::vector<std::string> copy;
    for(std::map<std::string, std::string>::iterator i = headers.begin(); i != headers.end(); ++i){
        std::string key = i->first;
        lowerString(key);
        copy.push_back(key);
    }
    if (std::find(copy.begin(), copy.end(), std::string("host")) == copy.end())
        return 400;

    if (headers.find("transfer-encoding") != headers.end()
        && headers["transfer-encoding"] != "chunked")
            return 501;
    
    if (method == "POST" && headers.find("transfer-encoding") == headers.end()
        && (headers.find("content-length") == headers.end()))
            return 411;
    return 0;
}

int ParseRequest::parseUri(std::string &uri) const {
    /*if => Request uri contain a character not allowded => bad request 400*/
    if (checkUriAllowedChars(uri) == false)
        return 400;
        // throw std::runtime_error("400 Bad Request : bad URI");

    if (uri.size() > MAX_SIZE)
        return 414;
 
    if (uri.find("/../") != std::string::npos|| uri.find("/..") != std::string::npos || uri.find("../") != std::string::npos)
        return 400;
    return 0;
}

int ParseRequest::parseVersion(std::string &version) const{
    if (version != "HTTP/1.1")
        return 501;
    // std::stringstream ss(version);
    
    // std::string http;
    // std::string vers;
    // std::getline(ss, http, '/');
    // std::getline(ss, vers);

    // char *endptr;
    // double v = strtod(vers.c_str(), &endptr);
  
    // if (http != "HTTP" || *endptr != '\0')
    //     // throw std::runtime_error("400 Bad Request : Error occured in Http version");
    //     return 400;
    
    // if (v == 0.9 || v == 2.0 || v == 1.0 || v == 3.0)
    //     // throw std::runtime_error("501 Not Implemented : Error occured in http version");
    //     return 501;
    
    // if (v != 1.1)
    //     // throw std::runtime_error("400 Bad Request : Error occured in Http version");
    //     return 400;
    return 0;
}

// void ParseRequest::parseBody(std::string &body, long long &maxBodySize) const{
//     if (static_cast<long long>(body.length()) > maxBodySize)
//         throw std::runtime_error("413 Request Entity Too Large");
// }

const std::deque<std::string> &ParseRequest::getHttpHeaders() const { return this->ngxHttpHeaders; }

int ParseRequest::checkUnknownHeader(std::map<std::string, std::string> &headers) const{
    std::map<std::string, std::string>::iterator it = headers.begin();
    for (; it != headers.end(); it++) {
    //     if(find(ngxHttpHeaders.begin(), ngxHttpHeaders.end(), it->first) == ngxHttpHeaders.end())
    //         return 400;
    //         // throw std::runtime_error("400 bad Request : unknown headers");
    }
    return 0;
}