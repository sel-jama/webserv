/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 00:44:16 by sel-jama          #+#    #+#             */
/*   Updated: 2024/03/31 06:54:10 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::handleMethod(Request &req, const infra &infras){
    Response obj;

    if (req.getMethod() == "GET")
        obj.handleGET(req, infras.getClients().at(0).ssocket);
}

void Response::handleGET(Request &req, int clientSocket) {
    GetHandler get;

    get.GetDataForClient(req, clientSocket);
}