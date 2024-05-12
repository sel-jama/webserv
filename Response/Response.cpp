/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 00:44:16 by sel-jama          #+#    #+#             */
/*   Updated: 2024/05/06 08:18:17 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::string Response::handleMethod(client &client){
    Response obj;

        if (client.reqq.getMethod() == "GET"){
            obj.handleGET(client.reqq);
        }
        else if (client.reqq.getMethod() == "DELETE")
            obj.handleDelete(client.reqq);

    return obj.response;
}

void Response::handleGET(Request &req){
    req.get.GetDataForClient(req);
    this->response = req.get.getResponse();
}

void Response::handleDelete(Request &req){
    Delete del;
    del.check_RequestedR(req);
}