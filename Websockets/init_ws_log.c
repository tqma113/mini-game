//
// Created by tianqi on 17-12-23.
//

#include "init_ws_log.h"

int init_ws_log(FILE **log_fd){
    char file[128] = "";
    strcpy(file, WSLOGFILE);
    if((*log_fd =fopen(file, "w")) == NULL){
        return -1;
    }
    printf("Log file init success...\n");
    return 1;
}