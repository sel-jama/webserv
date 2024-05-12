/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 11:13:12 by sel-jama          #+#    #+#             */
/*   Updated: 2024/05/06 22:11:22 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "ParseRequest.hpp"
// #include "../sock2/includes/client.hpp"
// #include "../sock2/includes/server.hpp"
#include "../sock2/includes/location.hpp"
#include "../Response/Method.hpp"
#include <algorithm>
#include <sys/time.h>


class client;
class server;
class infra;
// class location;
#define REQUEST_TIMEOUT 10

class Request {
    public:
        std::string method;
        std::string uri;
        std::string version;
        std::map<std::string, std::string> headers;
        std::map<std::string, std::string> extension;
        
    public:
        //additioanal
        std::string body;
        std::string reqStr;
        std::string queryString;
        std::string responseContentType;
        size_t responseContentLen;
        location matchedLocation;
        
        std::string getit;
        std::string r_s;
        int flag;
        std::string filname_s;
        // std::ofstream file;
        int flag2;
        size_t saver_count;
        int tmp;
        std::ofstream file; 
        std::string filename__;
        size_t size_body;
        size_t to_de;
        size_t to_de2;
        std::string content_T;

        std::string path;
        std::string fileName;
        // std::string bodySaver;
        struct stat pathStatus;
        size_t contentLength; 
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
        std::string response;
        size_t chunkPos;
        int firstChunk;
        std::string locationHeader;
        std::string cgi_File;
        std::string cgi_File2;
        Method get;
        time_t recentAction;
        int responseDone;
        std::string filePath;
        std::streampos filePosition;
        std::string responseHeaders;
        // double filePos;
        // std::string type;

        int r;
        Request();
        ~Request();

        //getters
        const std::string& getMethod(void) const;
        const std::string& getUri(void) const;
        const std::string& getVersion(void) const;
        const std::map<std::string, std::string>& getHeaders(void) const;
        const std::string& getBody(void) const;
        const std::string& getQuryString(void)const;

        //setters
        // void setMethod(const std::string );
        // void setUri(const std::string ) ;
        // void setVersion(const std::string );

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
        std::string readRequest(client &);
        void cutOffBodySegment(std::string &request);
        int send_response(client &);
        int read_request(client &, infra &);
        const server &getMatchedServer(uint16_t &, std::string &, const infra &);
        void uriQuery(std::string &uri);
        std::string generateResponse(client &, std::string &);
        void load_extension();
        std::string getMimeType(const std::string& fileName);
        void isRedirect();
        bool matchedName(const std::vector<std::string> &);
        bool checkReadingTimout(const client &client);
        char *readChunk(const char* filename);
        std::string getChunk(Request &req);
        void checkRequestLine(Request &saver, std::string &line);
        void checkSatusCode(Request &req);
};

#endif
