//
// Created by tianqi on 17-12-22.
//

#ifndef GAME_GAME_SERVER_H
#define GAME_GAME_SERVER_H

#include "init_info.h"
#include "../data_struct.h"
#include "../Websockets/websocket_server.h"

int start_game();

int deal_info(char info[32], struct PIPE *pipes);

int get_rand_char(char *info, int level);

#endif //GAME_GAME_SERVER_H
