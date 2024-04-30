/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 20:58:59 by sel-jama          #+#    #+#             */
/*   Updated: 2024/04/30 22:54:01 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errorPage.hpp"
#include "../Request/Request.hpp"
#include "../Response/method.hpp"

errorPage::errorPage(std::string msg, int code) : msg(msg), code(code) {
}

errorPage::errorPage(){
    setErrorMsgs();
}

errorPage::~errorPage(){}

void errorPage::setProprties(){
    switch (code){
        case 400 ... 499:
            color = "#ff4081";
            // boxRgba = "rgba(255, 64, 129, 0.5)";
            // img = "waiting.png";
            break;
        
        case 500 ... 599:
            color = "#167263";
            // boxRgba = "rgba(34, 144, 141, 0.5)";
            // img = "Hooray.jpeg";
            break;
        
        default:
            color = "#ff4081";
            // boxRgba = "rgba(255, 64, 129, 0.5)";
            // img = "/errorimg.png";
            break;
    }
    
}

void errorPage::HtmlErrorPage() {
    setProprties();
    // std::string html;

    
    
    // std::ifstream file("error.html");

    // if (!file.is_open())
    //     throw runtime_error("Something went wrong...");
    
    std::stringstream htmlCode;

    // Fill the stringstream with HTML code
    htmlCode << "<!DOCTYPE html>\n"
             << "<html>\n"
             << "<head>\n"
             << "    <title>Error</title>\n"
             << "    <style>\n"
             << "        body {\n"
             << "            background-color: black;\n"
             << "            color: #fff;\n"
             << "            text-align: center;\n"
             << "            font-family: \"Arial\", sans-serif;\n"
             << "            font-size: 30px;\n"
             << "            padding-top: 500px;\n"
             << "        }\n"
             << "        h1 {\n"
             << "            font-size: 48px;\n"
             << "            color: " << color << ";\n"
             << "            margin-bottom: 20px;\n"
             << "        }\n"
             << "        p {\n"
             << "            margin-bottom: 30px;\n"
             << "        }\n"
            //  << "        .error-img {\n"
            //  << "            width: 700px;\n"
            //  << "            height: auto;\n"
            //  << "            display: block;\n"
            //  << "            margin: 0 auto;\n"
            //  << "            border-radius: 100px;\n"
            //  << "            box-shadow: 0 0 20px "<< boxRgba << ";\n"
            //  << "        }\n"
             << "        .error-caption {\n"
             << "            font-style: italic;\n"
             << "            color: " << color << ";\n"
             << "        }\n"
             << "    </style>\n"
             << "</head>\n"
             << "<body>\n"
             << "    <h1>Error "<< code << "</h1>\n"
             << "    <p>" << msg << "</p>\n"
            //  << "    <img class=\"error-img\" src=\"" << img << "\" alt=\"error\">\n"
             << "    <p class=\"error-caption\">Lost in cyberspace...</p>\n"
             << "</body>\n"
             << "</html>\n";

    // Output the stringstream content
    // std::ofstream file("page.html", std::ios::trunc);

    // if (!file.is_open()){
    //     std::cout << "the problem is here" << std::endl;
    //     throw std::runtime_error("something went wrong, try freeing space");
    // }
    // file << htmlCode.str();
    this->html = htmlCode.str();
    
}

void errorPage::setErrorMsgs(){
    //html error pages
    errorMsgs[200] = "OK";
    errorMsgs[400] = "Bad Request";
    errorMsgs[401] = "Unquthorized";
    errorMsgs[403] = "Forbidden";
    errorMsgs[404] = "Not Found";
    errorMsgs[405] = "Method Not Allowed";
    errorMsgs[406] = "Not Acceptable";
    errorMsgs[407] = "Proxy Authentication Required";
    errorMsgs[408] = "Request Timeout";
    errorMsgs[409] = "Conflict";
    errorMsgs[410] = "Gone";
    errorMsgs[411] = "Length Required";
    errorMsgs[412] = "Precondition Failed";
    errorMsgs[413] = "Request Entity Too Large";
    errorMsgs[414] = "Request-URI Too Long";
    errorMsgs[415] = "Unsupported Media Type";
    errorMsgs[416] = "Requested Range Not Satisfiable";
    errorMsgs[417] = "Expectation Failed";
    errorMsgs[500] = "Internal Server Error";
    errorMsgs[501] = "Not Implemented";
    errorMsgs[502] = "Bad Gateway";
    errorMsgs[503] = "Service Unavailable";
}

// int main() {
//     errorPage test("Oops! Looks like the page you're looking for is missing.", 500);

//     try {
//         test.HtmlErrorPage();
//     }
//     catch (const std::runtime_error &e)
//     {
//         std::cerr << e.what() << std::endl;
//     }
//     return 0;
// }

// std::string errorPagesConfig(Request &req){
//     for ()
// }

std::string errorPage::serveErrorPage(Request &req){
    errorPage use;
    method use2;
    // std::string content;
    // std::ostringstream response;
    // use.setErrorMsgs();
    errorPage err(use.errorMsgs[req.errorCode], req.errorCode);
    std::cout << use.errorMsgs[req.errorCode] << std::endl;

    // std::string retErr = errorPagesConfig(req);
    // if ()

    try{
        err.HtmlErrorPage();
        // req.path = req.matchedLocation.root + req.matchedLocation.location_name + "sock2/page.html";
        // std::cout << "path error : " << req.path << std::endl;
        // // req.path = "../page.html";
        // content = use2.readContent(req);
        // response << "HTTP/1.1 " << req.errorCode << " KO\r\n"
        //     //  << "Content-Type: " << mimeType << "\r\n"
        //      << "Content-Length: " << content.length() << "\r\n"
        //      << "\r\n"
        //      << content;
    }
    catch(const std::runtime_error &e){
        std::cout << "errorPage failed :" << e.what() << std::endl;
    }
    // std::cout << err.html << std::endl;
    return err.html;
}