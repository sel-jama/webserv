/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleCgi.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 07:35:35 by sel-jama          #+#    #+#             */
/*   Updated: 2024/05/03 20:30:34 by sel-jama         ###   ########.fr       */
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

class Request;

class handleCgi {
    private:
        std::string scriptName;
        std::string cgiPath;
        pid_t pid;

public:
    handleCgi();
    // handleCgi(const std::map<std::string, std::string>& requestHeaders, const std::string& requestMethod, const std::string& requestPath, const std::string& requestQuery, const std::string& requestBody, const std::string& scriptName);
    std::string executeCgiScript(Request &req);
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
};

#endif