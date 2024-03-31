#include "Socket.hpp"

// Socket::Socket(int domain, int type, int protocol, int port, u_long ip){

//     address.sin_family = domain;
//     address.sin_port = htons(port);
//     address.sin_addr.s_addr = htonl(ip);
//     this->sock = socket(domain, type, protocol);
// }

// void Socket::clientConnect(char **av){
//     int sockfd, n;
//     int sendbytes;
//     struct sockaddr_in servaddr;
//     char sendline[MAXLINE];
//     char recvline[MAXLINE];

//     sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (sockfd < 0){
//         cerr << "Failed to create the socket" << endl;
//         exit(EXIT_FAILURE);
//     }

//     bzero(&servaddr, sizeof(servaddr));
//     servaddr.sin_family = AF_INET;
//     servaddr.sin_port = htons(SERVER_PORT);

//     if (inet_pton(AF_INET, av[1], &servaddr.sin_addr) <= 0){
//         cerr << "Failed in inet_pton" << endl;
//         exit(EXIT_FAILURE);
//     }

//     if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0){
//         cerr << "Failed to connect" << endl;
//         exit(EXIT_FAILURE);
//     }

//     //we're connected now ... prepare msg
//     sprintf(sendline, "GET / HTTP/1.1\r\n\r\n");
//     sendbytes = strlen(sendline);

//     if (write(sockfd, sendline, sendbytes) != sendbytes)
//     {
//         cerr << "Failed to write to the socket" << endl;
//         exit(1);
//     }

//     memset(recvline, 0, MAXLINE);

//     while ((n = read(sockfd, recvline, MAXLINE-1) > 0))
//         cout << recvline;

//     if (n < 0){
//         cerr << "Reading failed" << endl;
//         exit(1);
//     }
// }

char *bin2hex(const unsigned char *input, size_t len){
    char *res;
    string hexa = "0123456789QBCDEF";

    const char *hex = hexa.c_str();

    if (!input || len <= 0)
        return NULL;

    int reslen = len * 3 + 1;
    // res = new char[reslen];
    res = (char*)malloc(reslen);
    bzero(res, reslen);

    for(size_t i =0; i<len; i++){
        res[i*3]=hex[input[i] >> 4];
        res[(i*3)+1] = hex[input[i] & 0x0F];
        res[(i*3)+2] = ' ';
    }   
    return res;
}

void Socket::serverConnect(){
    int listenfd, connfd, n;
    struct sockaddr_in servaddr;
    uint8_t buff[MAXLINE+1];
    uint8_t recvline[MAXLINE+1];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if (listenfd < 0){
        cout << "Failed to creat server socket " << endl;
        exit(EXIT_FAILURE);
    }

    // bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVER_PORT);

    //listen and bind
    if ((bind(listenfd, (SA *) &servaddr, sizeof(servaddr))) < 0){
        cout << "Failed to bind" << endl;
        exit(EXIT_FAILURE);
    }

    if ((listen(listenfd, 10)) < 0){
        cout << "Listen failed" << endl;
        exit(EXIT_FAILURE);
    }

    while(1){
        // struct sockaddr_in addr;
        // socklen_t add_len;

        cout << "Waiting for connection on port " << SERVER_PORT << endl;
        // fflush(stdout);
        connfd = accept(listenfd, (SA *) NULL, NULL);

        memset(recvline, 0, MAXLINE);
        //read the client's message
        while (( n = read(connfd, recvline, MAXLINE-1) ) > 0){
            fprintf(stdout, "\n%s\n\n%s", bin2hex(recvline, n), recvline);

            //detect end of message
            if (recvline[n-1] == '\n')
                break;
            memset(recvline, 0, MAXLINE);
        }
        if (n < 0){
            cerr << "read failed" << endl;
            exit(EXIT_FAILURE);
        }

        //seding response
        snprintf((char*)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nHELLO WEB");

        write(connfd, (char*)buff, strlen((char*)buff));
        close(connfd);
    }
}