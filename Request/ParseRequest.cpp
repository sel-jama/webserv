/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:13:04 by sel-jama          #+#    #+#             */
/*   Updated: 2024/05/03 21:20:20 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseRequest.hpp"

ParseRequest::ParseRequest() {
    // setMethods();
    setUriAllowedChars();
    setHttpHeaders();
}

// GET /Hooray.jpeg HTTP/1.1
// Host: localhost:50000
// Connection: keep-alive
// sec-ch-ua: "Not/A)Brand";v="99", "Google Chrome";v="115", "Chromium";v="115"
// sec-ch-ua-mobile: ?0
// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36
// sec-ch-ua-platform: "macOS"
// Accept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8
// Sec-Fetch-Site: same-origin
// Sec-Fetch-Mode: no-cors
// Sec-Fetch-Dest: image
// Referer: http://localhost:50000/
// Accept-Encoding: gzip, deflate, br
// Accept-Language: fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7

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

// void ParseRequest::setMethods(void) {
//     this->methods.push_back("GET");
//     this->methods.push_back("POST");
//     this->methods.push_back("DELETE");
// }

void ParseRequest::setUriAllowedChars(void) {
    this->uriAllowedChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ._~:/?#[]@!$&'()*+,;=%";
}

// bool ParseRequest::checkMethod(const std::string &method) const {
//     if (method == "GET" || method == "POST" || method == "DELETE")
//         return "true";
//     return "false";
// }

// bool ParseRequest::unsupportedMethod(const std::string &method) const{
//     for (size_t i = 0; i < this->methods.size(); i++){
//         if (this->methods[i] == method)
//             return true;
//     }
//     return false;
// }

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
        // throw std::runtime_error("501 Not Implemented : Method not supported");
    return 0;
}

int ParseRequest::parseHeaders(std::map<std::string, std::string> &headers, std::string &method) const{
    int ret = checkUnknownHeader(headers);
    if (ret)
        return ret;
    
    if (headers.find("Transfer-Encoding") != headers.end()
        && headers["Transfer-Encoding"] != "chunked")
            // throw std::runtime_error("501 Not Implemented : Transfer-Encoding must be chuncked");
            return 501;
    
    char *end;
    if (method == "POST" && headers.find("Transfer-Encoding") == headers.end() && (headers.find("Content-Length") == headers.end() || strtod(headers.at("Content-Length").c_str(), &end) == 0))
        return 400;
        // throw std::runtime_error("400 Bad Request : Messing headers");
    
    //setContentLength(headers);
    return 0;
}

int ParseRequest::parseUri(std::string &uri) const {
    /*if => Request uri contain a character not allowded => bad request 400*/
    // if ((uri.size() >= 1 && uri[0] != '/') || checkUriAllowedChars(uri) == false)
    //     // throw std::runtime_error("400 Bad Request : bad URI");
    //     return 400;

    if (uri.size() > MAX_SIZE)
        // throw std::runtime_error("414 Request-URI Too Long");
        return 414;
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