/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 11:13:12 by sel-jama          #+#    #+#             */
/*   Updated: 2024/05/03 20:32:03 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "ParseRequest.hpp"
// #include "../sock2/includes/client.hpp"
// #include "../sock2/includes/server.hpp"
#include "../sock2/includes/location.hpp"
#include <algorithm>

class client;
class server;
class infra;
// class location;

class Request {
    //tochange
    public:
        std::string method;
        std::string uri;
        std::string version;
        std::map<std::string, std::string> headers;
        
        std::string body;

        // server serverInfo;
        // client &user;
        location matchedLocation;
        std::string reqStr;
        std::string responseContentType;
        int responseContentLen;
        std::string queryString;

        // infra Infra;

        //more to set up 
        // std::map<std::string, std::string> cgiEnv;
        // std::string query;
    public:
        //additioanal
        std::string getit;
        std::string r_s;
        int flag;
        size_t saver_count;
        int tmp;
        static std::ofstream file; 
        size_t to_de;

        std::string path;
        std::string fileName;
        // std::string bodySaver;
        struct stat pathStatus;
        double contentLength; 
        int readbytes;
        int readBody;
        int chunked_flag;
        int firstRead;
        int headersDone;
        //error utils
        int statusCode;
        std::string statusMsg;

        int isChunked;
        std::map<std::string, std::string>  errorPages;
        std::string ip;
        uint16_t port;
        uint16_t serverd;
        int cgi;

        int r;
        Request();
        ~Request();

        const std::string& getMethod(void) const;
        const std::string& getUri(void) const;
        const std::map<std::string, std::string>& getHeaders(void) const;
        const std::string& getBody(void) const;
        const std::string& getQuryString(void)const;

        void requestPartitioning(Request& saver, std::string& request);
        void isReqWellFormed(Request &req);
        void locateMatchingRequestURI(const server &use) const;
        bool allowedMethod(location &location) const;

        // static void getCheckRequest(Request &req, const server &serve, int &fdSock);
        static int getCheckRequest(client &, const infra &);

        std::string &getLocation(location &location) const;

        const location &getMatchingLocation(const server &serve) ;
        void retreiveRequestedResource(const server &serve) ;
        void isFileAvailable();
        void isMethodAllowed();

        // const server &getServerInfo(void) const;
        const location &getMatchedLocation(void) const;
        void setContentLength(const std::map<std::string, std::string> &headers);
        std::string readRequest(int &fdSocket);
        void cutOffBodySegment(std::string &request);
        int send_response(client &);
        int read_request(client &, infra &);
        const server &getMatchedServer(const infra &infra);
        void uriQuery(std::string &uri);
        std::string generateResponse(client &, std::string &);
};

#endif
