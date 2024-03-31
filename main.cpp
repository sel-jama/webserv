/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 00:38:49 by sel-jama          #+#    #+#             */
/*   Updated: 2024/03/31 00:50:00 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Socket.hpp"
#include "Response/Response.hpp"


int main(int ac, char **av){
    (void)av;
    (void)ac;
    
    // Socket s;
    // // s.clientConnect(av);
    // s.serverConnect();
    Request req;
    server  serve;
    int fd = 4;

    try {
        req = req.getCheckRequest(serve, fd);
        Response::handleMethod(req);
    }
    catch(std::runtime_error &e){
        std::cout << e.what() << std::endl;
    }
    return 0;
}
