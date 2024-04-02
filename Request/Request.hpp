/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 11:13:12 by sel-jama          #+#    #+#             */
/*   Updated: 2024/04/01 21:42:27 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "ParseRequest.hpp"

class Request {
    private:
        std::string method;
        std::string uri;
        std::string version;
        std::map<std::string, std::string> headers;
        std::string body;

        server serverInfo;
        location matchedLocation;
    public:
        //additioanal
        std::string path;
        std::string fileName;
        struct stat pathStatus;
        int contentLength;

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

        static void getCheckRequest(Request &req, server &serve, int &fdSock);
        std::string &getLocation(location &location) const;

        const location &getMatchingLocation(server &serve) ;
        void retreiveRequestedResource(server &serve) ;
        void isFileAvailable();

        const server &getServerInfo(void) const;
        const location &getMatchedLocation(void) const;
        void setContentLength(const std::map<std::string, std::string> &headers);

};

#endif
