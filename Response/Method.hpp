/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:13:16 by sel-jama          #+#    #+#             */
/*   Updated: 2024/03/31 07:29:15 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef METHOD_HPP
#define METHOD_HPP

// #include "../Request/Request.hpp"
#include "../Request/Global.hpp"
#include "../cgi/handleCgi.hpp"
#include "dirent.h"
#include <string>
class Request;
class server;
class location;

class Method {
    private:
        int code;
        std::string response;
        std::string type;  //file or dir
        bool isAutoIndex;
        std::map<std::string, std::string> mimeTypes;

        std::string content;  //retreived content
    public:
        handleCgi cgi;

        const std::string &getResponse() const {return this->response;}
        std::map<int, std::string> errorPage;
        
        void GetDataForClient(Request &);
        void retreiveRequestedResource(server &, Request &) const;
        const location &getMatchingLocation(server &, Request &);
        std::string readContent(Request &req) ;
        //setters

        void defineResourceType(Request &);
        void handleDirectory(Request &) ;
        bool isDirHasIndexFiles(Request &) const;
        void autoIndexing(Request &) ;
        void directoryListing(Request &) ;
        void validateAll(Request &req) const;
        void setErrorPages();
        bool loacationHasCgi(Request &req, handleCgi &cgi);
};

#endif