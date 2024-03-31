/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:13:16 by sel-jama          #+#    #+#             */
/*   Updated: 2024/03/31 06:49:27 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GETHANDLER_HPP
#define GETHANDLER_HPP

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../Request/Request.hpp"

class GetHandler {
    private:
    int code;
    std::string path;
    // std::string response;
    std::string type;
    bool isAutoIndex;

    public:
        void GetDataForClient(Request &, int &);
        void handleGetRequest(int clientSocket);
        void retreiveRequestedResource(server &, Request &) const;
        const location &getMatchingLocation(server &, Request &);
        std::string getMimeType(const std::string& fileName);
        std::string readContent(Request &req);

        //setters

        void defineResourceType(const Request &);
};

#endif