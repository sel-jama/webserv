/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 00:38:43 by sel-jama          #+#    #+#             */
/*   Updated: 2024/04/25 04:56:41 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include "../post/delete.hpp"
#include "Method.hpp"
#include "../Request/Request.hpp"
#include "../sock2/includes/client.hpp"
#include "../post/post.hpp"

class Response
{
private:
    std::string response;

public:
    Response(){}
    ~Response(){}

    static std::string handleMethod(client &client);
    void handleGET(Request &req);
    void handlePost(Request &req);
    void handleDelete(Request &req);
};

#endif