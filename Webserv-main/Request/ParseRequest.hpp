/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:13:24 by sel-jama          #+#    #+#             */
/*   Updated: 2024/03/29 00:54:54 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEREQUEST_HPP
#define PARSEREQUEST_HPP

#define BUFFER_SIZE 1024
#define MAX_SIZE 2048

#include <iomanip>
#include <vector>
#include <map>

class ParseRequest {
    private:
        static std::vector<std::string> methods;
    
    public:
        //utils
        void    setMethods(void);
        bool    checkMethod(const std::string &method) const;
        bool    unsupportedMethod(const std::string &method) const;

        //parse
        void parseMethod(std::string &method) const;
        void parseUri(std::string &uri) const;
        void parseVersion(std::string &version) const;
        void parseHeaders(std::map<std::string, std::string> &headers, std::string &method) const;
        void parseBody(std::string &body) const;
};

#endif