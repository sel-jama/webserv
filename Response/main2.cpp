/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 00:38:49 by sel-jama          #+#    #+#             */
/*   Updated: 2024/04/01 00:03:45 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "server/Socket.hpp"
#include "Response.hpp"


std::string main2(const infra &infras){
    
    // Socket s;
    // // s.clientConnect(av);
    // s.serverConnect();
    // Request req;
    server  serve = infras.getServer().at(0);
    int fd = infras.getClients().at(0).ssocket;
    (void)fd;

    Request req;
    Request::getCheckRequest(req, serve, fd);
    return (Response::handleMethod(req, infras));
}
