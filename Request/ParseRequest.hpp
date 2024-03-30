/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:13:24 by sel-jama          #+#    #+#             */
/*   Updated: 2024/03/29 21:44:28 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEREQUEST_HPP
#define PARSEREQUEST_HPP

#define BUFFER_SIZE 1024
#define MAX_SIZE 2048

#include <iomanip>
#include <vector>
#include <map>
#include <sstream>

class ParseRequest {
    private:
        std::vector<std::string> methods;
        std::string uriAllowedChars;
    
    public:
        //utils
        ParseRequest();
        void    setMethods(void);
        void    setUriAllowedChars();
        bool    checkMethod(const std::string &method) const;
        bool    unsupportedMethod(const std::string &method) const;
        bool    checkUriAllowedChars(std::string &uri) const;
        bool    charOccured(char c) const;

        //parse
        void parseMethod(std::string &method) const;
        void parseUri(std::string &uri) const;
        void parseVersion(std::string &version) const;
        void parseHeaders(std::map<std::string, std::string> &headers, std::string &method) const;
        void parseBody(std::string &body, long long &maxBodySize) const;
};

#endif