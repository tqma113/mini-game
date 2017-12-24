//
// Created by tianqi on 17-12-23.
//

#ifndef GAME_WEBSOCKET_SESSION_H
#define GAME_WEBSOCKET_SESSION_H

#include "../data_struct.h"

int deal_ws_request(int new_fd, struct sockaddr_in *client_addr);

int is_ws_request(char *request, struct ws_request* req);

int is_right_ws_method(bool *meth, char *method);

int is_right_ws_protocol(char *protocol);

#endif //GAME_WEBSOCKET_SESSION_H
