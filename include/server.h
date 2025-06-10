//
// Created by anuj on 10/6/25.
//

#pragma once
#include <string>
#include <netinet/in.h>
#include <thread>
#include <vector>
#include "threadpool.h"

class Server {
public:
    Server(int port, const std::string& www_root);
    void start();

private:
    int port;
    std::string www_root;
    int server_fd;
    sockaddr_in address;
    ThreadPool pool;
    void handleClient(int client_fd);
};
