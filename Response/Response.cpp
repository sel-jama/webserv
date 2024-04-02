/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 00:44:16 by sel-jama          #+#    #+#             */
/*   Updated: 2024/04/01 00:11:02 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::string Response::handleMethod(Request &req, const infra &infras){
    Response obj;

    if (req.getMethod() == "GET")
        obj.handleGET(req, infras.getClients().at(0).ssocket);
    
    return obj.response;
}

void Response::handleGET(Request &req, int clientSocket) {
    method get;

    get.GetDataForClient(req, clientSocket);
}