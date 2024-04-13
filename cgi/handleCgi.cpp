/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleCgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 07:40:50 by sel-jama          #+#    #+#             */
/*   Updated: 2024/04/13 14:47:10 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "handleCgi.hpp"

// void handleCGI(const std::string& cgiPath) {
//     int sockfd[2];
//     if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd) == -1) {
//         std::cerr << "Error creating socket pair" << std::endl;
//         return;
//     }

//     pid_t pid = fork();
//     if (pid == -1) {
//         std::cerr << "Error forking process" << std::endl;
//         return;
//     }

//     if (pid == 0){
//         close(sockfd[0]);

//         dup2(sockfd[1], STDOUT_FILENO);
//         close(sockfd[1]);

//         // Execute CGI script
//         if (execve(cgiPath.c_str(), NULL, NULL) == -1) {
//             std::cerr << "Error executing CGI script" << std::endl;
//             exit(EXIT_FAILURE);
//         }
//     }else{
//         close(sockfd[1]);

//         // Send output back to client
//         std::cout << "Content-Type: text/html\r\n\r\n"; // HTTP header
//         char buffer[1024];
//         ssize_t bytesRead;
//         while ((bytesRead = read(sockfd[0], buffer, sizeof(buffer))) > 0) {
//             std::cout.write(buffer, bytesRead);
//         }

//         close(sockfd[0]);
//     }
// }



std::string generateRandomFileName() {
    std::srand(std::time(NULL));
    
    const int filenameLen = 8;
    const std::string chars = "abcdefghijklmnopqrstuvwxyz0123456789";
    std::stringstream ss;

    for (int i = 0; i < filenameLen; ++i) {
        ss << chars[std::rand() % chars.length()];
    }
    return ss.str();
}

void CgiHandler::validateCgi(const Request &req){
    std::string exe;
    int found = 0;
    size_t pos = scriptName.find(".");
    
    exe = scriptName.substr(pos + 1);
    //exe == php
    std::map<std::string, std::string>::iterator it = req.matchedLocation.cgi.begin();
    for (; it != req.matchedLocation.cgi.end(); ++it){
        if (it.first == exe){
            cgiPath = it.second;
            found = 1;
            break;
        }
    }
    if (!found)
        throw std::runtime_error("set error page : requested cgi not in location")
}

char *const *arr &CgiHandler::createArr(){
    char *const *arr = new char *const[3];

    arr[0] = cgiPath.c_str();
    arr[1] = scriptName.c_str();
    arr[2] = NULL;

    return arr;
}

// std::string CgiHandler::executeCgiScript(const Request &req) {
//     scriptName = req.fileName;
//     validateCgi(req);
    
//     //create random file file name to not mix up clients files
//     std::string random = generateRandomFileName();
//     std::ifstream random("");
//     //check if random open

//     pid_t pid = fork();
//     if (pid == -1)
//         throw std::runtime_error("set error page : fork failed");
    
//     else if (pid == 0){

//         // Redirect stdin and stdout
//         (scriptName, STDIN_FILENO);
//         (random, STDOUT_FILENO);

//         //Execute the CGI script
//         char *const arr[] = createArr();
//         execve(cgiPath.c_str(), arr, NULL);
//         throw std::runtime_error("Error: Failed to execute CGI script");
            
//     } else {
//         close(pipeStdin[0]);
//         close(pipeStdout[1]);

//         // Write request body to stdin of CGI script
//         write(pipeStdin[1], body.c_str(), body.length());
//         close(pipeStdin[1]);

//         // Read output from stdout of CGI script
//         std::ostringstream output;
//         char buffer[BUFSIZ];
//         ssize_t bytesRead;
//         while ((bytesRead = read(pipeStdout[0], buffer, BUFSIZ)) > 0) {
//             output.write(buffer, bytesRead);
//         }
//         close(pipeStdout[0]);

//         // Wait for child process to finish
//         int status;
//         waitpid(pid, &status, 0);

//         // Free environment variables
//         for (size_t i = 0; env[i] != NULL; ++i) {
//             delete[] env[i];
//         }
//         delete[] env;

//         return output.str();
//     }
// }


std::string executeCgiScript(const Request &req) {
    scriptName = req.fileName;
    validateCgi(req);
    
    // Create random file name to avoid mixing up clients' files
    std::string random = generateRandomFileName();

    // Open random file for writing
    std::ofstream outputFile(random);
    if (!outputFile.is_open())
        throw std::runtime_error("Failed to open random file for writing");

    pid_t pid = fork();---
    if (pid == -1)
        throw std::runtime_error("Fork failed");
    
    else if (pid == 0) {
        // Redirect stdout to the random file
        if (freopen(random.c_str(), "w", stdout) == NULL) {
            throw std::runtime_error("Failed to redirect stdout");
        }

        // Execute the CGI script
        char *const arr[] = createArr();
        execve(cgiPath.c_str(), arr, NULL);
        throw std::runtime_error("Failed to execute CGI script");
        
    
    } else {
        // Close the file descriptor inherited by the child
        outputFile.close();
        // Wait for child process to finish
        int status;
        waitpid(pid, &status, 0);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            throw std::runtime_error("Child process failed to execute CGI script");
        }
    }
    return random;
}


