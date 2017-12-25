//
// Created by tianqi on 17-12-25.
//

#ifndef GAME_SESSION_H
#define GAME_SESSION_H

#include "../data_struct.h"

int webSocket_send(int fd, unsigned char *data, unsigned int dataLen, bool mod, Websocket_CommunicationType type);

int webSocket_recv(int fd, unsigned char *data, unsigned int dataMaxLen);

int webSocket_enPackage(unsigned char *data, unsigned int dataLen, unsigned char *package, unsigned int packageMaxLen, bool isMask, Websocket_CommunicationType type);

int webSocket_dePackage(unsigned char *data, unsigned int dataLen, unsigned char *package, unsigned int packageMaxLen, unsigned int *packageLen);

void webSocket_getRandomString(unsigned char *buf, unsigned int len);

#endif //GAME_SESSION_H
