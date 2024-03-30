/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GETmethod.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:13:01 by sel-jama          #+#    #+#             */
/*   Updated: 2024/03/30 01:14:06 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GETmethod.hpp"

void handle_get_request(int client_socket) {
    // Here, you would implement the logic to retrieve the requested resource
    // This could involve reading files from the server, querying a database, etc.
    // For simplicity, let's just send back a static response

    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";

    send(client_socket, response.c_str(), response.length(), 0);

    // Close the connection
    close(client_socket);
}

int sameUntilIndex(const std::string &uri, const std::string &locationName){
    size_t i = 0;
    while (uri[i] && locationName[i] && uri[i] == locationName[i])
        i++;
    return i;
}

const location &GETmethod::getMatchingLocation(server &serve, Request &req) {
    int counter;

    size_t i = 0;
    counter = sameUntilIndex(req.getUri(), serve.getLocations().at(i).location_name);
    i++;
    for (; i < serve.getLocations().size(); i++){
        if (sameUntilIndex(req.getUri(), serve.getLocations().at(0).location_name) > counter)
            counter = sameUntilIndex(req.getUri(), serve.getLocations().at(0).location_name);  
    }
    if (i >= serve.getLocations().size())
        throw std::runtime_error("404 Not found : No matching location");
    
    return serve.getLocations().at(i);
    
}

void GETmethod::retreiveRequestedResource(server &serve, Request &req) const{
    std::string path;
    const location &locate = getMatchingLocation(serve, req);
    //see the root of the location retrieved and join it with the uri then look for it using access
}

void GETmethod::handleClient(server &serve, Request &req, int &clientSocket) {
    char buffer[BUFFER_SIZE];
    retreiveRequestedResource(serve, req);
    int bytes_received = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received < 0) {
        std::cerr << "Error in receiving data from client\n";
        return ;
    }
    buffer[bytes_received] = '\0';

    handle_get_request(clientSocket);
}
