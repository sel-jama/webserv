/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 20:58:59 by sel-jama          #+#    #+#             */
/*   Updated: 2024/04/18 21:10:29 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errorPage.hpp"

errorPage::errroPage(std::string msg, int code) : msg(msg), code(code) {
}

errorPage::~errorPage(){}

void errorPage::HtmlErrorPage() {
    std::stringstream ss;

    ss  << "<html>"
        << "<head>"
        << "<style type=text/css>"

    ss << "</style>"
    ss << "</head>"

    ss << "<body>"

    ss << "</body>"
    ss << "</html>"
}