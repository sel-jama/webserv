# Webserv: A Custom HTTP Server in C++98

Welcome to the Webserv project! This repository contains our implementation of an HTTP server in C++98. The project aims to provide a deeper understanding of how HTTP works and why URLs start with "HTTP".

# Table of Contents
  - Simple Description
  - General Rules
  - Mandatory Part
  - Bonus Part

# Simple Description
Webserv is a comprehensive project aimed at helping developers grasp the complexities of the Hypertext Transfer Protocol (HTTP) by constructing their own HTTP server from scratch using C++98. This server will interact with actual web browsers, making it a practical and valuable learning experience.

### Key Features:
 * HTTP Protocol Mastery: Understand the intricacies of one of the most widely used protocols on the internet.
 * C++98 Compliance: Develop the server strictly using the C++98 standard, enhancing your proficiency in this version of C++.
 * Real-world Application: Test your server with actual web browsers to ensure it functions correctly in real-world scenarios.
 * Static and Dynamic Content: Serve both static content (like HTML pages and images) and dynamic content using CGI scripts.
 * Non-blocking Operations: Implement non-blocking I/O operations to handle multiple client requests efficiently.
 * Configuration Files: Customize the server behavior through configuration files, specifying settings like port numbers, server names, and routes.
 * Robust Error Handling: Provide accurate HTTP response status codes and default error pages for a professional server response.

This project not only deepens your understanding of HTTP but also boosts your coding skills in C++, preparing you for a wide range of software development challenges.

# General Rules
The server must not crash or quit unexpectedly under any circumstances.
A Makefile is provided to compile the source files.
Code must comply with the C++98 standard and be compiled with -Wall -Wextra -Werror flags.
No external libraries or Boost libraries are allowed.

# Mandatory Part

The server must handle configuration files and be non-blocking.
It should support GET, POST, and DELETE methods.
Compatibility with major web browsers is required.
Default error pages must be provided if none are specified.
Must be able to serve static websites and allow file uploads.

# Bonus Part
For additional features, consider implementing:

Support for cookies and session management.
Handling multiple CGI scripts.
