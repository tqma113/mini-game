//
// Created by tianqi on 17-12-22.
//

#include "game_server.h"


int start_game(){
    printf("Start game server...\n");
    int game_infor[64];
    char msg[MSG_MAX_SIZE];

    struct PIPE pipes[MAX_USER];
    for(int i = 0; i < MAX_USER; i++){
        //pipes[i]= (struct PIPE *)malloc(sizeof(struct PIPE));
        pipes[i].pipe[0] = 0;
        pipes[i].pipe[1] = 0;
        pipes[i].useState = false;
    }

    int shm_id;
    struct PIPE *shared = NULL;
    if((shm_id = shmget(IPC_PRIVATE, sizeof(struct PIPE) * MAX_USER, 0640 | IPC_CREAT | IPC_EXCL)) == -1){
        printf("Sharing memory cannot been created!\n");
        return -1;
    }
    printf("%d", shm_id);
    shared = (struct PIPE *)shmat(shm_id, NULL, 0);
    if(shared == -1){
        printf("Cannot link sharing memory to this process!(Game server)\n");
        return -1;
    }
    if(memcpy(shared, pipes,sizeof(struct PIPE) * MAX_USER) == NULL){
        printf("Cannot set sharing memory data!\n");
        return -1;
    }

    int game_pipe[2];
    pipe(game_pipe);

    int listen_fd = 0;
    int websocket_fd = 0;

    if(init_info(game_infor) == -1){
        printf("Game infor inti failure!\n");
        return -1;
    }
    if((websocket_fd = fork()) == 0){
        if(start_websocket(WEBSOCKET_PORT, shm_id, game_pipe[1]) == -1){
            printf("Websocket server start failure!\n");
            return -1;
        }
    }
    if((listen_fd = fork()) == 0){
        while(true){
            if(read(game_pipe[0], msg, MSG_MAX_SIZE) <= 0){
                printf("Pipe between webSocket and Game server is broken!\n");
                return -1;
            }
            printf("%s\n", msg);
        }
    }
    while(true){
        sleep(5);
        for(int i = 0; i < MAX_USER; i++){
            printf("\n%d : %d\n", i, (int)shared[i].useState);
            if(shared[i].useState == true){
                if(write(shared[i].pipe[1], "<Game> <mtq> 123", strlen("<Game> <mtq> 123")) == -1){
                    printf("The server pipe is broken!\n");
                    return -1;
                }
            }
        }
    }
}