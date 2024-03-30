/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:13:04 by sel-jama          #+#    #+#             */
/*   Updated: 2024/03/29 21:46:22 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseRequest.hpp"

ParseRequest::ParseRequest() {
    setMethods();
    setUriAllowedChars();
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
    if (headers.find("Transfer-Encoding") != headers.end()
        && headers["Transfer-Encoding"] != "chunked")
            throw std::runtime_error("501 Not Implemented : Transfer-Encoding must be chuncked");
    else if (headers.find("Content-Length") == headers.end() && method == "POST")
        throw std::runtime_error("400 Bad Request : Messing headers");
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
        throw std::runtime_error("400 Bad Request : Error occured in http version");
    
    if (v == 0.9 || v == 2.0 || v == 1.0 || v == 3.0)
        throw std::runtime_error("501 Not Implemented : Error occured in http version");
    
    if (v != 1.1)
        throw std::runtime_error("400 Bad Request : Error occured in http version");
}

void ParseRequest::parseBody(std::string &body, long long &maxBodySize) const{
    if (body.length() > maxBodySize)
        throw std::runtime_error("413 Request Entity Too Large");
}