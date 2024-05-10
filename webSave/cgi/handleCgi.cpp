/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleCgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 07:40:50 by sel-jama          #+#    #+#             */
/*   Updated: 2024/05/07 23:05:50 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "handleCgi.hpp"
#include "../Request/Request.hpp"
#include "../Response/Method.hpp"

handleCgi::handleCgi(){
}

void handleCgi::setScriptName(const std::string name){
    this->scriptName = name;
}

std::string handleCgi::generateRandomFileName() {
    std::srand(std::time(NULL));
    
    const int filenameLen = 8;
    const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::stringstream ss;

    for (int i = 0; i < filenameLen; ++i) {
        ss << chars[std::rand() % chars.length()];
    }
    return ss.str();
}

void handleCgi::validateCgi(const Request &req){
    std::string exe;
    int found = 0;
    size_t pos = scriptName.find(".");
    
    exe = scriptName.substr(pos + 1);
    //exe == php
    std::map<std::string, std::string>::const_iterator it = req.matchedLocation.cgi.begin();
    for (; it != req.matchedLocation.cgi.end(); ++it){
        if (it->first == exe){
            cgiPath = it->second;
            found = 1;
            break;
        }
    }
    if (!found)
        throw std::runtime_error("set error page : requested cgi not in location");
}

char ** handleCgi::createArr() {
    char **arr = reinterpret_cast<char **>( malloc( 3 * sizeof( char * ) ) );
    arr[0] = strdup( this->cgiPath.c_str() );
    arr[1] = strdup( this->scriptName.c_str() );
    arr[2] = NULL;
    // if ( !arr[0] || !arr[1] ) {
    //     return ( 500 ); 
    // }
    return arr;
}

char **handleCgi::createGetEnv(Request &req){
    std::map<std::string, std::string> mapEnv;
    
	std::map<std::string, std::string>	headers = req.getHeaders();

    mapEnv["REDIRECT_STATUS"] = "200";
    mapEnv["SCRIPT_NAME"] = req.path;
    mapEnv["SCRIPT_FILENAME"] = scriptName;
    mapEnv["REQUEST_METHOD"] = "GET";
    mapEnv["QUERY_STRING"] = req.getQuryString();
	mapEnv["SERVER_PROTOCOL"] = "HTTP/1.1";
	mapEnv["SERVER_SOFTWARE"] = "Webserv/1.0";
    mapEnv["HTTP_COOKIE"] = headers["Cookie"];

    
    char	**env = new char*[mapEnv.size() + 1];
	int	j = 0;
	for (std::map<std::string, std::string>::const_iterator i = mapEnv.begin(); i != mapEnv.end(); i++) {
		std::string	element = i->first + "=" + i->second;
		env[j] = new char[element.size() + 1];
		env[j] = strcpy(env[j], (const char*)element.c_str());
		j++;
	}
	env[j] = NULL;
    return env;
}

void trimcgi(std::string &str) {
    // Trim leading whitespace
    while (!str.empty() && std::isspace(str[0]))
        str.erase(0, 1);

    // Trim trailing whitespace
    while (!str.empty() && std::isspace(str[str.length() - 1]))
        str.erase(str.length() - 1);
}


std::string handleCgi::parseCgiRsponse(std::string &cgiOutput){
    std::string ret;
    std::stringstream ss("");
    int generateHeaders = 0;
    std::stringstream iss("");
    
    size_t p = cgiOutput.find("\r\n\r\n");
    if (p != std::string::npos)
    {
        std::string headers = cgiOutput.substr(0, p + 4);
        std::string body = cgiOutput.substr(p+4);
        if (headers.empty() || headers == "\r\n\r\n")
            generateHeaders = 1;
        else
        {
            std::map<std::string, std::string> mapHeaders;
            iss << headers;
            std::string headerLine("");
            while (std::getline(iss, headerLine, '\n'))
            {
                if (headerLine == "\r")
                    break;
                size_t pos = headerLine.find(':');
                if (pos != std::string::npos)
                {
                    std::string key = headerLine.substr(0, pos);
                    std::string value = headerLine.substr(pos + 1);
                    trimcgi(key);
                    trimcgi(value);
                    if (key.empty() || value.empty()){
                        generateHeaders = 1;
                    }
                    mapHeaders[key] = value;
                }
                else
                    generateHeaders = 1;
            }
            if (!generateHeaders){
                if (mapHeaders.find("Content-Length") == mapHeaders.end()){
                    ss << "Content-Length: " << body.length() << "\r\n";
                }
                ss << cgiOutput;
                ret = ss.str();     
            }
        }
    }
    if (p == std::string::npos || generateHeaders){
        //generate response with headers
        ss << "Content-Length: " << cgiOutput.length() << "\r\n"
            << "Content-Type: text/plain\r\n\r\n"
            << cgiOutput;
        ret = ss.str();
    }
    return ret;
}

void handleCgi::executeCgiScript(Request &req) {
    req.cgi = 1;
    scriptName = req.path;
    // validateCgi(req);
    std::string response;
    std::string output;
    
    // Create random file name to avoid mixing up clients' files
    random = generateRandomFileName();

    // Open random file for writing
    std::ofstream outputFile(random.c_str());
    if (!outputFile.is_open()){
        req.statusCode = 500;
        throw std::runtime_error("Failed to open random file for writing");
    }

    pid = fork();
    if (pid == -1){
        req.statusCode = 500;
        throw std::runtime_error("Fork failed");
    }
    
    else if (pid == 0) {
        if (freopen(random.c_str(), "w", stdout) == NULL){
            req.statusCode = 500;
            throw std::runtime_error("Failed to redirect stdout");
        }
        char **const arr = createArr();
        char **const env = createGetEnv(req);
        execve(cgiPath.c_str(), arr, env);
        req.statusCode = 500;
        throw std::runtime_error("Failed to execute CGI script");
        
    } else {
        counter = time(NULL);
        outputFile.close();
        checkTimeout(req);
    }
}

void handleCgi::checkTimeout(Request &req){
        int status;
        int result = waitpid(pid, &status, WNOHANG);
            if (result == -1){
                // Handle waitpid error
                kill(pid, SIGKILL);
                req.statusCode = 500;
                throw std::runtime_error("Error waiting for CGI script");
            }else if(result > 0){
                if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
                    std::remove(random.c_str());
                    req.statusCode = 500;
                    throw std::runtime_error("Child process failed to execute CGI script");
                }
                if (req.method != "POST"){
                    Method use;
                    req.path = req.matchedLocation.root + "/" + random;
                    std::string output = use.readContent(req);
                    response = parseCgiRsponse(output);
                    std::cout << "CGI output :   \n" << response << std::endl;
                    std::remove(random.c_str());
                }
                else{
                    req.statusCode = 201;
                    //set response content type
                    //set response content len
                } 
            }
            else {
                if(time(NULL) - counter >= CGI_TIMEOUT){
                    kill(pid, SIGKILL);
                    std::remove(random.c_str());
                    req.statusCode = 504;
                    throw std::runtime_error("CGI timeout");
                }
            }
}

//POST CGI >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

char **handleCgi::createPostEnv(Request &req){
    std::map<std::string, std::string> mapEnv;
    
	std::map<std::string, std::string>	headers = req.getHeaders();
	if (headers.find("Auth-Scheme") != headers.end() && headers["Auth-Scheme"] != "")
		mapEnv["AUTH_TYPE"] = headers["Authorization"];

	mapEnv["REDIRECT_STATUS"] = "201";
	mapEnv["SCRIPT_NAME"] = req.path;
	mapEnv["SCRIPT_FILENAME"] = scriptName;
	mapEnv["REQUEST_METHOD"] = "POST";
    std::ostringstream oss;
    oss << req.responseContentLen;
	mapEnv["CONTENT_LENGTH"] = oss.str();
	mapEnv["CONTENT_TYPE"] = req.responseContentType;
	mapEnv["SERVER_NAME"] = headers["Hostname"];
	mapEnv["SERVER_PORT"] = req.port;
	mapEnv["SERVER_PROTOCOL"] = "HTTP/1.1";
    mapEnv["PATH_INFO"] = req.path;
    mapEnv["HTTP_COOKIE"] = headers["Cookie"];
    
    char	**env = new char*[mapEnv.size() + 1];
	int	j = 0;
	for (std::map<std::string, std::string>::const_iterator i = mapEnv.begin(); i != mapEnv.end(); i++) {
		std::string	var = i->first + "=" + i->second;
		env[j] = new char[var.size() + 1];
		env[j] = strcpy(env[j], (const char*)var.c_str());
		j++;
	}
	env[j] = NULL;
    return env;
}

void handleCgi::executeCgiBody(Request &req){
    req.cgi = 1;
    setScriptName(req.cgi_File);
    std::cout << "Cgi post is ready to run .. " << std::endl;
    // std::string randomFile = generateRandomFileName();
    // std::string pathtofile = req.matchedLocation.upload_path + "/" + randomFile;
    
    std::cout << "req.cgi_File2.c_str() :" << req.cgi_File2.c_str() << std::endl;
    std::ofstream outputFile(req.cgi_File2.c_str());
    if (!outputFile.is_open()){
        req.statusCode = 500;
        throw std::runtime_error("failed to open file ...");
    }

    pid_t pid = fork();
    if (pid == -1){
        req.statusCode = 500;
        throw std::runtime_error("fork failed");
    }

    else if (pid == 0){
        std::string cgiBody = req.cgi_File;
        std::cout << "file >>> " << req.cgi_File << std::endl;
        if (freopen(cgiBody.c_str(), "r", stdin) == NULL
            || freopen(req.cgi_File2.c_str(), "w", stdout) == NULL){
            req.statusCode = 500;
            throw std::runtime_error("failed to redirect input..");
        }

        char **const arr = createArr();
        char **const env = createPostEnv(req);
        execve(this->cgiPath.c_str(), arr, env);
        req.statusCode = 500;
        throw std::runtime_error("execve failed");
    } else {
        counter = time(NULL);
        outputFile.close();
        checkTimeout(req);
    }
}


