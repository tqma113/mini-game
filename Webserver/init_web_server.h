//
// Created by tianqi on 17-12-22.
//

#ifndef GAME_INIT_SERVER_H
#define GAME_INIT_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#define PACKAGE_SIZE 5

int init_server(int *server_fd, struct sockaddr_in *server, int port);

#endif //GAME_INIT_SERVER_H
