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

    // try{
        //std::cout << client.reqq.getMethod()  << ".....********************"<< std::endl;
        if (client.reqq.getMethod() == "GET"){
            obj.handleGET(client.reqq);
        }
        else if (client.reqq.getMethod() == "POST"){

            std::cout << "hello im here !" << std::endl;
            
            obj.handlePost(client.reqq);
        }
        else if (client.reqq.getMethod() == "DELETE")
            obj.handleDelete(client.reqq);
    // }
    // catch(const std::runtime_error &e){
    //         return "";
    // }
    return obj.response;
}

void Response::handleGET(Request &req){
    // method get;
    
    req.get.GetDataForClient(req);
    // if (req.cgi){
    //     get.cgi.checkTimeout(req);
    //     if (!get.cgi.response.empty())
    //         this->response = get.cgi.response;
    //     else{
    //         req.cgi = 0;
    //         req.statusCode = 500;
    //         throw std::runtime_error("something went wrong");
    //     }
    // }
 
        this->response = req.get.getResponse();
}

void Response::handlePost(Request &req){
    Post post;
    Method use;
    if(!req.isChunked)
    {
        std::cout << "Wa lharba" << std::endl;
        post.support_upload(req);
        this->response = use.readContent(req);
    }
}

void Response::handleDelete(Request &req){
    Delete del;

    del.check_RequestedR(req);
}