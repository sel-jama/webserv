/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 00:38:43 by sel-jama          #+#    #+#             */
/*   Updated: 2024/04/11 11:25:51 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include "method.hpp"
#include "../sock2/includes/infra.hpp"

class Response
{
private:
    std::string response;

public:
    Response(){}
    ~Response(){}

    static std::string handleMethod(Request &req, const infra &infras);
    void handleGET(Request &req, int);
};

void main2(const infra &infra, Request &req, int fd);

#endif