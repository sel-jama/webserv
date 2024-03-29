/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:13:04 by sel-jama          #+#    #+#             */
/*   Updated: 2024/03/29 00:57:34 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseRequest.hpp"

void ParseRequest::setMethods(void) {
    this->methods.push_back("PUT");
    this->methods.push_back("PATCH");
    this->methods.push_back("HEAD");
    this->methods.push_back("OPTIONS");
    this->methods.push_back("TRACE");
    this->methods.push_back("CONNECT");
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
    if (uri.size() >= 1 && uri[0] != '/')
        throw ("400 Bad Request : bad URI");

    if (uri.size() > MAX_SIZE)
        throw std::runtime_error("414 Request-URI Too Long");
}