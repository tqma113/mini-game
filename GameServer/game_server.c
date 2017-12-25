//
// Created by tianqi on 17-12-22.
//

#include "game_server.h"


int start_game(int write_fd, int read_fd){
    printf("Start game server...\n");
    int game_infor[MSG_MAX_SIZE];
    char msg[64];

    int listen_fd = 0;

    if(init_info(game_infor) == -1){
        printf("Game infor inti failure!\n");
        return -1;
    }
    if((listen_fd = fork()) == 0){
        while(true){
            if(read(read_fd, msg, MSG_MAX_SIZE) <= 0){
                printf("Pipe between webSocket and Game server is broken!\n");
                return -1;
            }
            printf("%s\n", msg);
        }
    }
    while(true){
        sleep(5);
        if(write(write_fd, "<Game> <mtq> 123", strlen("<Game> <mtq> 123")) == -1){
            printf("The server pipe is broken!\n");
            return -1;
        }
    }
}