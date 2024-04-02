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
        // int code;
        std::string path;
        std::string response;
        std::string type;
        bool isAutoIndex;
        std::map<std::string, std::string> mimeTypes;
        

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
        void autoIndexing(Request &) const ;
};

#endif