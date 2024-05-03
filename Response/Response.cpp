/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 00:44:16 by sel-jama          #+#    #+#             */
/*   Updated: 2024/05/03 20:51:55 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::string Response::handleMethod(client &client){
    Response obj;

    try{
        //std::cout << client.reqq.getMethod()  << ".....********************"<< std::endl;
        if (client.reqq.getMethod() == "GET"){
            obj.handleGET(client.reqq, client.ssocket);
        }
        else if (client.reqq.getMethod() == "POST"){
            std::cout << "hello im here !" << std::endl;
            
            obj.handlePost(client.reqq);
        }
    }
    catch(const std::runtime_error &e){
            return "";
    }
    return obj.response;
}

void Response::handleGET(Request &req, int clientSocket) {
    method get;
    
        get.GetDataForClient(req, clientSocket);

        this->response = get.getResponse();
}

void Response::handlePost(Request &req){
    Post post;
    method use;
    if(!req.isChunked)
    {
        // std::cout << "jaaaaaaaa "<< std::endl;
        post.support_upload(req);
        this->response = use.readContent(req);
    }
} 