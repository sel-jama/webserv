/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 00:38:43 by sel-jama          #+#    #+#             */
/*   Updated: 2024/03/31 07:06:40 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include "../Request/Request.hpp"
#include "GetHandler.hpp"
#include "../sock2/includes/infra.hpp"

class Response
{
private:
    std::string response;

public:
    Response(){}
    ~Response(){}

    static void handleMethod(Request &req, const infra &infras);
    void handleGET(Request &req, int);
};

int main2(const infra &infra);

#endif