/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleCgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 07:40:50 by sel-jama          #+#    #+#             */
/*   Updated: 2024/04/04 07:45:58 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "handleCig.hpp"

void execute_cgi(const char* script_path, const char* request_method, const char* query_string, const char* content_length, const char* content_type) {
    char buf[MAX_BUF_SIZE];
    FILE* script_output;
    int content_length_int = atoi(content_length);

    // Set up environment variables
    setenv("REQUEST_METHOD", request_method, 1);
    setenv("QUERY_STRING", query_string, 1);
    setenv("CONTENT_LENGTH", content_length, 1);
    setenv("CONTENT_TYPE", content_type, 1);

    // Execute CGI script
    
    script_output = popen(script_path, "r");
    if (!script_output) {
        cerr << "Failed to execute CGI script." << endl;
        return;
    }

    // Read script output and send to client
    while (fgets(buf, sizeof(buf), script_output) != NULL) {
        cout << buf;
    }

    pclose(script_output);
}