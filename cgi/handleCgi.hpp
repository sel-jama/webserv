/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleCgi.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 07:35:35 by sel-jama          #+#    #+#             */
/*   Updated: 2024/05/04 01:03:06 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLECGI_HPP
#define HANDLECGI_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>
#include <signal.h>
#define CGI_TIMEOUT 9

class Request;

class handleCgi {
    private:
        std::string scriptName;
        std::string cgiPath;
        pid_t pid;
        time_t counter;
        std::string random;

public:
    std::string response;
    handleCgi();
    void executeCgiScript(Request &req);
    char** createArr();
    char** createEnv();
    void validateCgi(const Request &req);
    
    void setScriptName(const std::string );
    
    static void handleTimeout(int);
    std::string generateRandomFileName();
    char ** createPostEnv(Request &req);
    void executeCgiBody(Request &req);
    char **createGetEnv(Request &req);
    std::string parseCgiRsponse(std::string &);
    void checkTimeout(Request &req);
};

#endif