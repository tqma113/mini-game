//
// Created by tianqi on 17-12-22.
//

#include "init_log.h"

int init_log(FILE *log_fd){
    char file[128] = "";
    strcpy(file, ROOT_DIR);
    strcat(file, LOGFILE);
    if((log_fd =fopen(file, "w")) == NULL){
        return -1;
    }
    printf("Log file init success...\n");
    return 0;
}