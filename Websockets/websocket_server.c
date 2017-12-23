//
// Created by tianqi on 17-12-22.
//

#include "init_websocket_server.h"
#include "init_ws_log.h"
#include "websocket_session_init.h"
#include "websocket_server.h"

int start_websocket(int port){
    int server_fd = 0;
    int new_fd = 0;
    FILE *log_fd;
    pid_t client_fd = 0;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t socketaddr_in_size = 0;
    if(init_ws_server(&server_fd, &server_addr, port) == -1){
        perror("Server start error!");
        return -1;
    }
    if(init_ws_log(&log_fd) == -1){
        perror("Log process cannot run!");
        return -1;
    }
    socketaddr_in_size = sizeof(struct sockaddr_in);
    printf("Websocket server start success!\n");
    while(true){
        if((new_fd = accept(server_fd, (struct sockaddr *) &client_addr, &socketaddr_in_size)) == -1){
            perror("Cannot accpet request!");
            printf("pid %d loss connection to %s\n", getpid(), inet_ntoa(client_addr.sin_addr));
            close(server_fd);
            return -1;
        }
        if((client_fd = fork()) > 0){
            close(new_fd);
            continue;
        } else if(client_fd == 0){
            //close(server_fd);
            printf("\npid %d process Websocket session from %s : %d\n", getpid(), inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port));
            fprintf(log_fd, "pid %d process Websocket session from %s : %d\n", getpid(), inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port));
            printf("Start deal request %d of %s:%d\n", new_fd, inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port));
            if(deal_ws_request(new_fd, &client_addr) == -1){
                perror("Deal request failure!");
                break;
            }
            printf("pid %d close Websocket connection to %s\n", getpid(), inet_ntoa(client_addr.sin_addr));
            fprintf(log_fd, "pid %d close Websocket connection to %s\n", getpid(), inet_ntoa(client_addr.sin_addr));
            close(new_fd);
        } else {
            perror("Cannot create dealing process!");
            break;
        }
    }

    close(server_fd);
    return 1;
}