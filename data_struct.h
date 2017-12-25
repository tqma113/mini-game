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
#include <sys/un.h>
#include <sys/epoll.h>  // epoll管理服务器的连接和接收触发


#include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

#include <pthread.h>    // 使用多线程

// websocket根据data[0]判别数据包类型    比如0x81 = 0x80 | 0x1 为一个txt类型数据包
typedef enum{
    WCT_MINDATA = -20,      // 0x0：标识一个中间数据包
    WCT_TXTDATA = -19,      // 0x1：标识一个txt类型数据包
    WCT_BINDATA = -18,      // 0x2：标识一个bin类型数据包
    WCT_DISCONN = -17,      // 0x8：标识一个断开连接类型数据包
    WCT_PING = -16,     // 0x8：标识一个断开连接类型数据包
    WCT_PONG = -15,     // 0xA：表示一个pong类型数据包
    WCT_ERR = -1,
    WCT_NULL = 0
}Websocket_CommunicationType;


#define WEBSERVER_PORT 8080
#define WEBSOCKET_PORT 9090

#define ROOT_DIR "/home/tianqi/CLionProjects/Game/FrontPage"
#define INDEX_FILE "/index.html"

#define PACKAGE_SIZE 5

#define HEAD_MAX_SIZE 1024
#define LINE_MAX_SIZE 1024

#define TIME_OUT_SEC 10
#define TIME_OUT_USEC 0

#define WSLOGFILE "./ws_log.txt"

#define LOGFILE "./log.txt"

#define CATALOGUE "Catalogue"

#define READ_BUF_SIZE 4096       //1k = 4096
#define FILE_BUF_SIZE 1048576     //4096 * 64
#define SEND_BUF_SIZE 2097152     //4096 * 128

#define URI_MAX_SIZE 128

//#define GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

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
    char host[32];
    bool connection_is_up;
    bool upgrade_is_ws;
    char origin[32];
    int sec_websocket_version;
    char accept_language[32];
    char sec_websocket_key[64];
    char sec_websocket_extensions[32];
};

#endif //GAME_DATA_STRUCT_H
