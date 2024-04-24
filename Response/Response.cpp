/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 00:44:16 by sel-jama          #+#    #+#             */
/*   Updated: 2024/04/24 07:00:00 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::string Response::handleMethod(client &client){
    Response obj;

    if (client.reqq.getMethod() == "GET"){
        obj.handleGET(client.reqq, client.ssocket);
    }
    // else if (client.reqq.getMethod() == "POST")
        
    
    // std::cout << "now"<<obj.response << std::endl;
    return obj.response;
}

void Response::handleGET(Request &req, int clientSocket) {
    method get;

    get.GetDataForClient(req, clientSocket);

    this->response = get.getResponse();
}
