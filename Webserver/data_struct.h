//
// Created by tianqi on 17-12-22.
//

#ifndef GAME_DATA_STRUCT_H
#define GAME_DATA_STRUCT_H

#define ROOT_DIR "/home/tianqi/CLionProjects/Game/FrontPage"
#define INDEX_FILE "/index.html"


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



#endif //GAME_DATA_STRUCT_H
