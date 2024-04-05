/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleCgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 07:40:50 by sel-jama          #+#    #+#             */
/*   Updated: 2024/04/05 11:50:13 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "handleCig.hpp"

void handleCGI(const std::string& cgiPath) {
    int sockfd[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd) == -1) {
        std::cerr << "Error creating socket pair" << std::endl;
        return;
    }

    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Error forking process" << std::endl;
        return;
    }

    if (pid == 0){
        close(sockfd[0]);

        dup2(sockfd[1], STDOUT_FILENO);
        close(sockfd[1]);

        // Execute CGI script
        if (execve(cgiPath.c_str(), NULL, NULL) == -1) {
            std::cerr << "Error executing CGI script" << std::endl;
            exit(EXIT_FAILURE);
        }
    }else{
        close(sockfd[1]);

        // Send output back to client
        std::cout << "Content-Type: text/html\r\n\r\n"; // HTTP header
        char buffer[1024];
        ssize_t bytesRead;
        while ((bytesRead = read(sockfd[0], buffer, sizeof(buffer))) > 0) {
            std::cout.write(buffer, bytesRead);
        }

        close(sockfd[0]);
    }
}
