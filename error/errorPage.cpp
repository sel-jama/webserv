/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 20:58:59 by sel-jama          #+#    #+#             */
/*   Updated: 2024/05/06 22:21:55 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errorPage.hpp"
#include "../Request/Request.hpp"
#include "../Response/Method.hpp"

errorPage::errorPage(std::string msg, int code) : msg(msg), code(code) {
}

errorPage::errorPage(){
    setstatusMsgs();
}

errorPage::~errorPage(){}

void errorPage::setProprties(){
    switch (code){
        case 400 ... 499:
            color = "#ff4081";
            break;
        
        case 500 ... 599:
            color = "#167263";
            break;
        
        default:
            color = "#ff4081";
            break;
    }
    
}

void errorPage::HtmlErrorPage() {
    setProprties();
    
    std::stringstream htmlCode;

    htmlCode << "<!DOCTYPE html>\n"
             << "<html>\n"
             << "<head>\n"
             << "    <title>Error</title>\n"
             << "    <style>\n"
             << "        body {\n"
             << "            background-color: black;\n"
             << "            color: #fff;\n"
             << "            font-size: 30px;\n"
             << "        }\n"
             << "        h1 {\n"
             << "            font-size: 48px;\n"
             << "            color: " << color << ";\n"
             << "        }\n"
             << "        .error-caption {\n"
             << "            font-style: italic;\n"
             << "            color: " << color << ";\n"
             << "        }\n"
             << "    </style>\n"
             << "</head>\n"
             << "<body>\n"
             << "    <h1>Error "<< code << "</h1>\n"
             << "    <p>" << msg << "</p>\n"
             << "    <p class=\"error-caption\">Lost in cyberspace...</p>\n"
             << "</body>\n"
             << "</html>\n";

    this->html = htmlCode.str();
}

void errorPage::setstatusMsgs(){
    //html error pages
    statusMsgs[200] = "OK";
    statusMsgs[201] = "Created";
    statusMsgs[202] = "Accepted";
    statusMsgs[203] = "Non-Authoritative Information";
    statusMsgs[204] = "No Content";
    statusMsgs[205] = "Reset Content";
    statusMsgs[206] = "Partial Content";
    statusMsgs[300] = "Multiple Choices";
    statusMsgs[301] = "Moved Permanently";          
    statusMsgs[302] = "Found";
    statusMsgs[303] = "See Other";
    statusMsgs[304] = "Not Modified";
    statusMsgs[305] = "Use Proxy";
    statusMsgs[307] = "Temporary Redirect";
    statusMsgs[400] = "Bad Request";
    statusMsgs[401] = "Unquthorized";
    statusMsgs[403] = "Forbidden";
    statusMsgs[404] = "Not Found";
    statusMsgs[405] = "Method Not Allowed";
    statusMsgs[406] = "Not Acceptable";
    statusMsgs[407] = "Proxy Authentication Required";
    statusMsgs[408] = "Request Timeout";
    statusMsgs[409] = "Conflict";
    statusMsgs[410] = "Gone";
    statusMsgs[411] = "Length Required";
    statusMsgs[412] = "Precondition Failed";
    statusMsgs[413] = "Request Entity Too Large";
    statusMsgs[414] = "Request-URI Too Long";
    statusMsgs[415] = "Unsupported Media Type";
    statusMsgs[416] = "Requested Range Not Satisfiable";
    statusMsgs[417] = "Expectation Failed";
    statusMsgs[500] = "Internal Server Error";
    statusMsgs[501] = "Not Implemented";
    statusMsgs[502] = "Bad Gateway";
    statusMsgs[503] = "Service Unavailable";
    statusMsgs[504] = "Gateway Timeout";
    statusMsgs[505] = "HTTP Version Not Supported";
}

std::string toString(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

std::string errorPagesConfig(Request &req){
    std::string err = toString(req.statusCode);
    std::cout << "here" << req.errorPages[err]  << std::endl;
    if (!req.errorPages[err].empty()){
        std::cout << req.errorPages[err] << std::endl;
        std::string pathToerrorPage = req.errorPages[err];
        struct stat status;
        if (stat(pathToerrorPage.c_str(), &status) == 0 && status.st_mode &S_IRUSR) //error page file found && permission
            return pathToerrorPage;
    }
    return "";
}

std::string errorPage::serveErrorPage(Request &req){
    errorPage use;
    Method use2;
    // std::string content;
    // std::ostringstream response;
    // use.setstatusMsgs();
    errorPage err(use.statusMsgs[req.statusCode], req.statusCode);

    std::string pathToPage = errorPagesConfig(req);
    std::cout << "hi " << pathToPage << std::endl;
    if (!pathToPage.empty()){
        req.path = pathToPage;
        return use2.readContent(req);
    }

    err.HtmlErrorPage();

    return err.html;
}