//
// Created by tianqi on 17-12-22.
//

#ifndef GAME_WEBSOCKET_SERVER_H
#define GAME_WEBSOCKET_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int start_websocket(int port);

#endif //GAME_WEBSOCKET_SERVER_H
