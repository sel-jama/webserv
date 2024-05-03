/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleCgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 07:40:50 by sel-jama          #+#    #+#             */
/*   Updated: 2024/05/02 23:44:40 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "handleCgi.hpp"
#include "../Request/Request.hpp"
#include "../Response/method.hpp"

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
    // char const**arr = new char const*[3];

    // // Assuming cgiPath and scriptName are std::string objects
    // arr[0] = cgiPath.c_str();
    // arr[1] = scriptName.c_str(); //path
    // arr[2] = NULL;

    char **arr = reinterpret_cast<char **>( malloc( 3 * sizeof( char * ) ) );
    arr[0] = strdup( this->cgiPath.c_str() );
    arr[1] = strdup( this->scriptName.c_str() );
    arr[2] = NULL;
    // if ( !arr[0] || !arr[1] ) {
    //     return ( 500 ); 
    // }
    return arr;
}

std::string handleCgi::executeCgiScript(Request &req) {
    scriptName = req.path;
    // validateCgi(req);
    std::string response;
    method use;
    
    // Create random file name to avoid mixing up clients' files
    std::string random = generateRandomFileName();

    // Open random file for writing
    std::ofstream outputFile(random);
    if (!outputFile.is_open()){
        req.statusCode = 500;
        throw std::runtime_error("Failed to open random file for writing");
    }

    pid_t pid = fork();
    if (pid == -1){
        req.statusCode = 500;
        throw std::runtime_error("Fork failed");
    }
    
    else if (pid == 0) {
        // Redirect stdout to the random file
        if (freopen(random.c_str(), "w", stdout) == NULL){
            req.statusCode = 500;
            throw std::runtime_error("Failed to redirect stdout");
        }
        // Execute the CGI script
        char **const arr = createArr();
        // const char *arr[] = {cgiPath.c_str(), scriptName.c_str() ,NULL};
        execve(cgiPath.c_str(), arr, NULL);
        req.statusCode = 500;
        throw std::runtime_error("Failed to execute CGI script");
        
    } else {
        // Close the file descriptor inherited by the child
        outputFile.close();

        // Set a maximum execution time (adjust as needed)
        int max_execution_time = 10; // Seconds

        int status;
        int remaining_time = max_execution_time;
        while (remaining_time > 0 && (waitpid(pid, &status, WNOHANG)) == 0) {
            // Child process hasn't finished yet
            sleep(1); // Adjust sleep duration for finer control
            remaining_time--;
        }

        if (remaining_time == 0) {
            // Timeout occurred
            // Terminate child process forcefully
            kill(pid, SIGKILL);
            req.statusCode = 504;
            throw std::runtime_error("CGI script execution timed out");
        }

        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            req.statusCode = 500;
            throw std::runtime_error("Child process failed to execute CGI script");
        }
    }
    req.path = req.matchedLocation.root + req.matchedLocation.location_name + "sock2/" + random;
    response = use.readContent(req);
    return response;
}


// void handleCgi::executeCgiBody(Request &req) {
//     std::string randomFile = generateRandomFileName();
    
//     std::ofstream outputFile(randomFile);
//     if (!outputFile.is_open()){
//         req.statusCode = 500;
//         throw std::runtime_error("failed to open file ...");
//     }

//     pid_t pid = fork();
//     if (pid == -1){
//         req.statusCode = 500;
//         throw std::runtime_error("fork failed");
//     }

    // else if (pid == 0){
        // if (freopen(randomFile.c_str(), "r", ))
    // }
// }



