//
// Created by tianqi on 17-12-23.
//

#ifndef GAME_INIT_WEBSOCKET_SERVER_H
#define GAME_INIT_WEBSOCKET_SERVER_H

#include "../data_struct.h"

#define PACKAGE_SIZE 5

int init_ws_server(int *server_fd, struct sockaddr_in *server, int port);

#endif //GAME_INIT_WEBSOCKET_SERVER_H
