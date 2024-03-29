/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 11:13:12 by sel-jama          #+#    #+#             */
/*   Updated: 2024/03/28 23:59:17 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <fstream>
#include <unistd.h>
#include "ParseRequest.hpp"
#include "sock2/includes/server.hpp"

class Request {
    private:
        std::string method;
        std::string uri;
        std::string version;
        std::map<std::string, std::string> headers;
        std::string body;
    
    public:
        Request();
        ~Request();

        const std::string& getMethod(void) const;
        const std::string& getUri(void) const;
        const std::map<std::string, std::string>& getHeaders(void) const;
        const std::string& getBody(void) const;

        void requestPartitioning(Request& saver, const std::string& request);
        void isReqWellFormed(Request &req) const;
        void locateMatchingRequestURI(const server &use) const;
        bool allowedMethod(location &location) const;

        void getCheckRequest(const server &serve, int &fdSock);
        std::string &getLocation(location &location) const;
};

#endif