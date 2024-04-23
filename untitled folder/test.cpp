#include "select.hpp"
#include "../sock2/includes/client.hpp"
#include "../sock2/includes/server.hpp"
#include "../Request/Request.hpp"


int main()
{
    client c;
    server s;
    location l;
    try 
    {

    int opt = 1;
    struct sockaddr_in data;
    const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nHello";
    char buffer[1024];
    int socketest = socket(PF_INET, SOCK_STREAM, 0);
    data.sin_addr.s_addr = INADDR_ANY;
    data.sin_port = htons(50000);
    data.sin_family = AF_INET;
    if (setsockopt(socketest, SOL_SOCKET, SO_REUSEPORT, &(opt), sizeof(opt)) == -1) throw(std::runtime_error("Error: init servers1 : setsockopt() SO_REUSPORT"));
    if (bind(socketest, (const sockaddr *)&data, sizeof(data)) == -1)                         throw(std::runtime_error("Error: init servers1 : bind()"));
    if (listen(socketest, 10) == -1)                                                throw(std::runtime_error("Error: init servers1 : listen()"));
    std::cout << "Waiting for cnx ..." << std::endl;
    int newsock = accept(socketest, NULL, NULL);
    std::cout << "New connection!!"<< std::endl;
    c.ssocket = newsock;

    s.clientMaxBodySize = 2000000000000;
    l.location_name = "/";
    l.autoindex = "on";
    l.root = "/nfs/homes/again/";
    l.http_methods["GET"] = 1;

    s.locations.push_back(l);
    //set location

    // read(newsock, buffer, sizeof(buffer));
    Request::getCheckRequest(c, s);
    // std::string request = c.reqq.method;
    // request += " ";
    // request += c.reqq.uri;
    // request += " ";
    // request += c.reqq.version;
    // request += "\n";

    // request += c.reqq.headers;
    std::cout << c.reqq.reqStr << std::endl;
    c.reqq.send_response(c);
    // write(newsock, response, strlen(response));
    }
     catch(const std::exception &e)
    {
        std::cout <<"new main : " << e.what() << std::endl;
    }
}