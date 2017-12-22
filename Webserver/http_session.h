//
// Created by tianqi on 17-12-21.
//

#ifndef GAME_HTTP_SESSION_H
#define GAME_HTTP_SESSION_H

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
#include "data_struct.h"
#include <sys/types.h>
#include <dirent.h>

#define TIME_OUT_SEC 10
#define TIME_OUT_USEC 0

#define CATALOGUE "Catalogue"

#define READ_BUF_SIZE 4096       //1k = 4096
#define FILE_BUF_SIZE 1048576     //4096 * 64
#define SEND_BUF_SIZE 2097152     //4096 * 128

#define URI_MAX_SIZE 128

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
