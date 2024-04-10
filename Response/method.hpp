/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:13:16 by sel-jama          #+#    #+#             */
/*   Updated: 2024/03/31 07:29:15 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef METHOD_HPP
#define METHOD_HPP

#include "../Request/Request.hpp"
#include "dirent.h"

class method {
    private:
        int code;
        //std::string path;   //path to dir or file
        std::string response;
        std::string type;  //file or dir
        bool isAutoIndex;
        std::map<std::string, std::string> mimeTypes;

        std::string content;  //retreived content
        

    public:
        void GetDataForClient(Request &, int &);
        void handleGetRequest(int clientSocket);
        void retreiveRequestedResource(server &, Request &) const;
        const location &getMatchingLocation(server &, Request &);
        // std::string getMimeType(const std::string& fileName);
        std::string readContent(Request &req) ;

        //setters

        void defineResourceType(const Request &);
        void handleDirectory(Request &) ;
        bool isDirHasIndexFiles(Request &) const;
        void autoIndexing(Request &) const ;
        void directoryListing(Request &) const;
};

#endif