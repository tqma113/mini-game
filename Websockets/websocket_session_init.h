//
// Created by tianqi on 17-12-23.
//

#ifndef GAME_WEBSOCKET_SESSION_H
#define GAME_WEBSOCKET_SESSION_H

#define HEAD_MAX_SIZE 1024

#define TIME_OUT_SEC 10
#define TIME_OUT_USEC 0

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>


struct ws_request{
    int client_socket;
    int state;
    bool method_right_state;
    char protocol[16];
    int port;
    char address[16];
    char file[64];
    char uri[4096];
};


int deal_ws_request(int new_fd, struct sockaddr_in *client_addr);

int is_ws_request(char *request, struct ws_request* req);

int is_right_ws_method(bool *meth, char *method);

int is_right_ws_protocol(char *protocol);

#endif //GAME_WEBSOCKET_SESSION_H
