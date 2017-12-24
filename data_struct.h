//
// Created by tianqi on 17-12-22.
//

#ifndef GAME_DATA_STRUCT_H
#define GAME_DATA_STRUCT_H

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

#define WEBSERVER_PORT 8080
#define WEBSOCKET_PORT 9090

#define ROOT_DIR "/home/tianqi/CLionProjects/Game/FrontPage"
#define INDEX_FILE "/index.html"

#define PACKAGE_SIZE 5

#define HEAD_MAX_SIZE 1024

#define TIME_OUT_SEC 10
#define TIME_OUT_USEC 0

#define WSLOGFILE "./ws_log.txt"

#define LOGFILE "./log.txt"

#define CATALOGUE "Catalogue"

#define READ_BUF_SIZE 4096       //1k = 4096
#define FILE_BUF_SIZE 1048576     //4096 * 64
#define SEND_BUF_SIZE 2097152     //4096 * 128

#define URI_MAX_SIZE 128

enum METHOD{
    GET = 1,
    POST,
    HEAD,
    PUT,
    DELETE,
    OPTIONS,
    TRACE,
    CONNECT
};

enum MIMETYPE{
    HTML=1,
    CSS,
    JS,
    JPG,
    PNG,
    JPEG,
    AVI,
    GIF,
    MP4,
    MP3,
    TXT
};

enum FILETYPE{
    NDIR = 1,
    NFILE,
    ROOT
};

struct http_request{
    int client_socket;
    int state;
    enum METHOD method;
    char protocol[16];
    int port;
    char address[16];
    char file[64];
    char uri[4096];
    enum MIMETYPE m_type;
    enum FILETYPE f_type;
};



struct ws_request{
    int client_socket;
    int state;
    bool method_right_state;
    char protocol[16];
    int port;
    char address[16];
    char uri[4096];
};

#endif //GAME_DATA_STRUCT_H
