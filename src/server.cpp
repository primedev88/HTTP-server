//
// Created by anuj on 10/6/25.
//

#include "server.h"
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <fcntl.h>

Server::Server(int port, const std::string& www_root):port(port), www_root(www_root), pool(4) {
    server_fd = socket(AF_INET, SOCK_STREAM,0);
    if(server_fd == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address))<0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 10)<0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout<<"Server listening on port "<<port<<std::endl;
}

void Server::start() {
    while(true){
        socklen_t addrlen = sizeof(address);
        int client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (client_fd<0){
            perror("accept");
            continue;
        }

        pool.enqueue([this, client_fd]() {
            this->handleClient(client_fd);
            close(client_fd);
        });
    }
}

void Server::handleClient(int client_fd) {
    char buffer[4096] = {0};
    read(client_fd, buffer, sizeof(buffer));
    std::cout<<"Received:\n"<<buffer<<std::endl;

    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
                           "<html><body><h1>Welcome to Pro HTTP Server!</h1></body></html>";
    send(client_fd, response.c_str(), response.size(), 0);
}