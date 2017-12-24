//
// Created by tianqi on 17-12-22.
//

#ifndef GAME_INIT_SERVER_H
#define GAME_INIT_SERVER_H

#include "../data_struct.h"

int init_server(int *server_fd, struct sockaddr_in *server, int port);

#endif //GAME_INIT_SERVER_H
