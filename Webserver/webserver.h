//
// Created by tianqi on 17-12-22.
//

#ifndef GAME_WEBSERVER_H
#define GAME_WEBSERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "init_server.h"
#include "init_log.h"
#include "http_session.h"

int start_web(int port);

#endif //GAME_WEBSERVER_H
