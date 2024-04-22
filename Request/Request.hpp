/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 11:13:12 by sel-jama          #+#    #+#             */
/*   Updated: 2024/04/22 04:11:32 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "ParseRequest.hpp"
#include "../sock2/includes/client.hpp"
#include "../sock2/includes/client.hpp"
#include "../sock2/includes/server.hpp"
#include <algorithm>

class client;
class server;

class Request {
    private:
        std::string method;
        std::string uri;
        std::string version;
        std::map<std::string, std::string> headers;
        // std::string body;

        // server serverInfo;
        // client &user;
        location matchedLocation;
        // infra Infra;

        //more to set up 
        // std::map<std::string, std::string> cgiEnv;
        // std::string query;
    public:
        //additioanal
        std::string path;
        std::string fileName;
        struct stat pathStatus;
        int contentLength; 
        int readbytes;
        int readBody;
        std::string bodySaver;

        Request();
        ~Request();

        const std::string& getMethod(void) const;
        const std::string& getUri(void) const;
        const std::map<std::string, std::string>& getHeaders(void) const;
        const std::string& getBody(void) const;

        void requestPartitioning(Request& saver, std::string& request);
        void isReqWellFormed(Request &req, long long maxBodySize) const;
        void locateMatchingRequestURI(const server &use) const;
        bool allowedMethod(location &location) const;

        // static void getCheckRequest(Request &req, const server &serve, int &fdSock);
        static int getCheckRequest(client &, const server &serve);

        std::string &getLocation(location &location) const;

        const location &getMatchingLocation(const server &serve) ;
        void retreiveRequestedResource(const server &serve) ;
        void isFileAvailable();

        const server &getServerInfo(void) const;
        const location &getMatchedLocation(void) const;
        void setContentLength(const std::map<std::string, std::string> &headers);

        std::string readRequest(int &fdSocket);
        void cutOffBodySegment(std::string &request);
        int send_response(client &);
};

#endif
