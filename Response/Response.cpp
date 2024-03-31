/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 00:44:16 by sel-jama          #+#    #+#             */
/*   Updated: 2024/03/31 01:14:51 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::handleMethod(Request &req){
    Response obj;

    if (req.getMethod() == "GET")
        obj.handleGET(req);
}

void Response::handleGET(Request &req) {
    GetHandler get;
    int clientSocket;

    get.GetDataForClient(req.getServerInfo(), req, clientSocket);
}