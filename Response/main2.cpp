/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 00:38:49 by sel-jama          #+#    #+#             */
/*   Updated: 2024/04/10 22:42:50 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "server/Socket.hpp"
#include "Response.hpp"


void main2(const infra &infras, Request &req, int fd){
    
    // Socket s;
    // // s.clientConnect(av);
    // s.serverConnect();
    // Request req;
    const server  &serve = infras.getServer().at(0);
    // int fd = infras.getClients().at(0).ssocket;

    Request::getCheckRequest(req, serve, fd);
    //fd_w send
    Response::handleMethod(req, infras);
}
