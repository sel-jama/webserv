/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GETmethod.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:13:16 by sel-jama          #+#    #+#             */
/*   Updated: 2024/03/30 01:14:09 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GETMETHOD_HPP
#define GETMETHOD_HPP

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "Request/Request.hpp"

class GETmethod {
    private:
    int _code;
    std::string _path;
    std::string _response;
    std::string _type;
    //bool _isAutoIndex;

    public:
        void handleClient(server &, Request &, int &);
        void handleGetRequest(int clientSocket);
        void retreiveRequestedResource(server &, Request &) const;
        const location &getMatchingLocation(server &, Request &);
};

#endif