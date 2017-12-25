//
// Created by tianqi on 17-12-23.
//

#ifndef GAME_WEBSOCKET_SESSION_H
#define GAME_WEBSOCKET_SESSION_H

#include "../data_struct.h"

#include "base64.h"

#include "sha1.h"

int deal_ws_request(int new_fd, struct sockaddr_in *client_addr, int write_fd, int read_fd);

int is_ws_request(char *request, struct ws_request* req);

int is_http_head(char *f_line, struct ws_request *req);

int is_right_ws_method(bool *meth, char *method);

int is_right_ws_protocol(char *protocol);

int _readline(char* allbuf,int level,char* linebuf);

int analysis_ws_info(char *request,  int index, struct ws_request *req);

int do_ws_response(struct ws_request *req);

int get_ws_first_line(char *f_line);

int get_upgreade_line(char *f_line);

int get_connection_line(char *f_line);

int get_sec_WA_line(char *f_line, char *key);

int webSocket_buildRespondShakeKey(unsigned char *acceptKey, unsigned int acceptKeyLen, unsigned char *respondKey);

int get_sec_WP_line(char *f_line, char *name);

#endif //GAME_WEBSOCKET_SESSION_H
