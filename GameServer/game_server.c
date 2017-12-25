//
// Created by tianqi on 17-12-22.
//

#include "game_server.h"


int start_game(){
    printf("Start game server...\n");
    int game_infor[64];

    if(init_info(game_infor) == -1){
        printf("Game infor inti failure!\n");
        return -1;
    }
    return 1;
}