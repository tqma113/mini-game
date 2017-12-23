//
// Created by tianqi on 17-12-22.
//

#include "init_web_server.h"

int init_server(int *server_fd, struct sockaddr_in *server, int port){
    if((*server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Server create falure...\n");
        return -1;
    }
    int reuse = 1;
    if(setsockopt(*server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse))){
        perror("Time set error!");
        return -1;
    }
    server->sin_family=AF_INET;
    server->sin_port=htons(port);
    server->sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(*server_fd, (const struct sockaddr *)server, sizeof(struct sockaddr_in)) < 0){       //端口绑定
        printf("Port has been used!\n");
        return -1;
    }
    if(listen(*server_fd, PACKAGE_SIZE)==-1){                                            //监听端口
        perror("Listening server error!\n");
        return -1;
    }
    printf("Server start...\n");
    return 0;
}
