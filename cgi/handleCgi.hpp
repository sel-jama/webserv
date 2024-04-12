/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleCgi.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 07:35:35 by sel-jama          #+#    #+#             */
/*   Updated: 2024/04/12 22:12:17 by sel-jama         ###   ########.fr       */
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

class CgiHandler {
    private:
        std::string scriptName;
        std::string cgiPath;

    void initializeEnvironment(const std::map<std::string, std::string>& requestHeaders, const std::string& requestMethod, const std::string& requestPath, const std::string& requestQuery, const std::string& requestBody, const std::string& scriptName);
    char** createEnvironmentArray() const;

public:
    CgiHandler(const std::map<std::string, std::string>& requestHeaders, const std::string& requestMethod, const std::string& requestPath, const std::string& requestQuery, const std::string& requestBody, const std::string& scriptName);
    std::string executeCgiScript(const Request &req);
    char *const *arr &createArr();
};

#endif