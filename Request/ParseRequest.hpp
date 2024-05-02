/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:13:24 by sel-jama          #+#    #+#             */
/*   Updated: 2024/05/02 17:54:15 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEREQUEST_HPP
#define PARSEREQUEST_HPP

#define BUFFER_SIZE 1024
#define MAX_SIZE 2048

#include "Global.hpp"

class ParseRequest {
    private:
        std::vector<std::string> methods;
        std::string uriAllowedChars;
        std::deque<std::string> ngxHttpHeaders;
    
    public:
        //utils
        ParseRequest();
        // void    setMethods(void);
        void    setUriAllowedChars();
        bool    checkMethod(const std::string &method) const;
        bool    unsupportedMethod(const std::string &method) const;
        bool    checkUriAllowedChars(std::string &uri) const;
        bool    charOccured(char c) const;

        //parse
        int parseMethod(std::string &method) const;
        int parseUri(std::string &uri) const;
        int parseVersion(std::string &version) const;
        int parseHeaders(std::map<std::string, std::string> &headers, std::string &method) const;
        // int parseBody(std::string &body, long long &maxBodySize) const;

        void setHttpHeaders();
        const std::deque<std::string> &getHttpHeaders() const;
        int checkUnknownHeader(std::map<std::string, std::string> & ) const; 
};

#endif