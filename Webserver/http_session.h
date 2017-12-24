//
// Created by tianqi on 17-12-21.
//

#ifndef GAME_HTTP_SESSION_H
#define GAME_HTTP_SESSION_H


#include "../data_struct.h"

int deal_request(int new_fd, struct sockaddr_in *client_addr);

int is_http_request(char *request, struct http_request* req);

int is_right_method(enum METHOD *meth, char *method);

int is_right_type(enum MIMETYPE *type, char *filename);

int is_right_protocol(char *protocol);

void div_type(struct http_request *req);

int access_file(struct http_request *req, char *file_buf);

char *weekday_to_str(int n);

char *int_to_string(int n);

char *get_current_time();

char *type_to_str(enum MIMETYPE type);

char *get_first_line(int state);

char *get_Date_line();

char *get_server_line();

char *get_contentE_line();

char *get_contentL_line();

char *get_contentLen_line(int len);

char *get_contentT_line(enum MIMETYPE type);

int get_dir_info(char *file_buf, char *dir, struct http_request *req);

int send_normal_msg(struct http_request *req, char *send_buf, int send_byte);

int send_error_msg(int state, char *send_buf);

#endif //GAME_HTTP_SESSION_H
