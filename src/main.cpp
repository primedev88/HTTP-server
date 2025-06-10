//
// Created by anuj on 10/6/25.
//

#include "server.h"

int main(){
    Server server(8080,  "./www");
    server.start();
    return 0;
}