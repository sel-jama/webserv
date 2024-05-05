/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 20:58:59 by sel-jama          #+#    #+#             */
/*   Updated: 2024/05/04 22:12:38 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errorPage.hpp"
#include "../Request/Request.hpp"
#include "../Response/method.hpp"

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
            //  << "            text-align: center;\n"
            //  << "            font-family: \"Arial\", sans-serif;\n"
             << "            font-size: 30px;\n"
            //  << "            padding-top: 100px;\n"
             << "        }\n"
             << "        h1 {\n"
             << "            font-size: 48px;\n"
             << "            color: " << color << ";\n"
            //  << "            margin-bottom: 20px;\n"
             << "        }\n"
            //  << "        p {\n"
            //  << "            margin-bottom: 30px;\n"
            //  << "        }\n"
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

std::string errorPagesConfig(Request &req){
    std::map<std::string, std::string>::iterator it = req.errorPages.begin();
    for(; it != req.errorPages.end(); ++it){
        // return req.matchedLocation.root +"/404.html";
        if (req.statusCode ==  atoi(it->first.c_str())){
            it->second = "404.html";
            std::string pathToerrorPage = req.matchedLocation.root + "/" + it->second;
            struct stat status;
            if (stat(pathToerrorPage.c_str(), &status) == 0 && status.st_mode &S_IRUSR) //error page file found && permission
                return pathToerrorPage;
        }
    }
    return "";
}

std::string errorPage::serveErrorPage(Request &req){
    errorPage use;
    method use2;
    // std::string content;
    // std::ostringstream response;
    // use.setstatusMsgs();
    errorPage err(use.statusMsgs[req.statusCode], req.statusCode);
    std::cout << use.statusMsgs[req.statusCode] << std::endl;

    std::string pathToPage = errorPagesConfig(req);
    std::cout << pathToPage << "<<<<<<<" << std::endl;
    if (!pathToPage.empty()){
        req.path = pathToPage;
        return use2.readContent(req);
    }

    try{
        err.HtmlErrorPage();
        // req.path = req.matchedLocation.root + req.matchedLocation.location_name + "sock2/page.html";
        // std::cout << "path error : " << req.path << std::endl;
        // // req.path = "../page.html";
        // content = use2.readContent(req);
        // response << "HTTP/1.1 " << req.statusCode << " KO\r\n"
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