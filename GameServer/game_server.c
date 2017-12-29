//
// Created by tianqi on 17-12-22.
//

#include "game_server.h"


int start_game(){
    printf("Start game server...\n");
    int game_infor[64];

    struct PIPE pipes[MAX_USER];
    for(int i = 0; i < MAX_USER; i++){
        //pipes[i]= (struct PIPE *)malloc(sizeof(struct PIPE));
        pipe(pipes[i].pipe);
        pipes[i].useState = false;
        pipes[i].level = 0;
        memset(pipes[i].score, 0 ,MAX_SONG);
        strcpy(pipes[i].name, "");
    }

    int shm_id;
    struct PIPE *shared = NULL;
    if((shm_id = shmget(IPC_PRIVATE, sizeof(struct PIPE) * MAX_USER, 0640 | IPC_CREAT | IPC_EXCL)) == -1){
        printf("Sharing memory cannot been created!\n");
        return -1;
    }
    shared = (struct PIPE *)shmat(shm_id, NULL, 0);
    if(shared == (struct PIPE *)-1){
        printf("Cannot link sharing memory to this process!(Game server)\n");
        return -1;
    }
    if(memcpy(shared, pipes,sizeof(struct PIPE) * MAX_USER) == NULL){
        printf("Cannot set sharing memory data!\n");
        return -1;
    }

    int game_pipe[2];
    pipe(game_pipe);

    int websocket_fd = 0;

//    if(init_info(game_infor) == -1){
//        printf("Game infor inti failure!\n");
//        return -1;
//    }
    if((websocket_fd = fork()) == 0){
        if(start_websocket(WEBSOCKET_PORT, shm_id, game_pipe[1]) == -1){
            printf("Websocket server start failure!\n");
            return -1;
        }
    }

    while(true){
        char msg[MSG_MAX_SIZE];
        memset(msg, 0, MSG_MAX_SIZE);
        if(read(game_pipe[0], msg, MSG_MAX_SIZE) <= 0){
            printf("Pipe between webSocket and Game server is broken!\n");
            return -1;
        }
        if(deal_info(msg, shared) == -1){
            printf("The info:%s is illegal!\n", msg);
            continue;
        }
    }
}

int deal_info(char *info, struct PIPE *pipes){
    char head[8];
    char para[8];
    char value[16];
    int new_n = 0;
    sscanf(info, "%[^_]_%[^_]_%s", head, para, value);
    if(strcmp(head, "Game") != 0){
        printf("Info head is wrong:%s!\n", head);
        return -1;
    }
    if(strcmp(para, "new") == 0){
        new_n = atoi(value);
        char buf[MSG_MAX_SIZE]= "info:";
        get_rand_char(buf, pipes[new_n].level);
        write(pipes[new_n].pipe[1],buf, 256);
        if(pipes[new_n].index > MAX_SONG)
            pipes[new_n].index = 1;
        pipes[new_n].index++;
    } else if(strcmp(para, "update") == 0){
        char user[16];
        char val[8];
        sscanf(value, "%[^_]_%s", user, val);
        if(strcmp(val, "s") == 0){
            for(int i = 0; i < MAX_USER; i++){
                if(pipes[i].useState && strcmp(user, pipes[i].name) == 0){
                    pipes[i].score[pipes[i].index] += PER_SORCE * pipes[i].level;
                    char update_info[256] = "update_";
                    char b[16];
                    sprintf(b, "%d", pipes[i].score[pipes[i].index]);
                    strcat(update_info, pipes[i].name);
                    strcat(update_info, "_");
                    strcat(update_info, b);
                    for(int j = 0; j < MAX_USER; j++){
                        if(pipes[j].useState == true){
                            if(write(pipes[j].pipe[1], update_info, strlen(update_info)) == -1){
                                printf("Pipe is broken!\n");
                                continue;
                            }
                        }
                    }
                    break;
                }
            }

        }
    } else if(strcmp(para, "chat") == 0){
        char chat_info[256] = "chat_";
        strcat(chat_info, value);
        for(int j = 0; j < MAX_USER; j++){
            if(pipes[j].useState == true) {
                if (write(pipes[j].pipe[1], chat_info, strlen(chat_info)) == -1) {
                    printf("Pipe is broken!\n");
                    continue;
                }
            }
        }
    } else if(strcmp(para, "get") == 0){
        char get_info[128] = "get_";
        char src[16];
        char dest[16];
        sscanf(value, "%[^_]%s", src, dest);
        for(int i = 0; i < MAX_USER; i++){
            if(strcmp(pipes[i].name, src) == 0){
                for (int j = 0; j < MAX_USER ; ++j) {
                    if(strcmp(pipes[j].name, dest) == 0){
                        if(write(pipes[i].pipe[1], get_info, strlen(get_info)) == -1){
                            printf("Pipe is broken!\n");
                            continue;
                        }
                        break;
                    }
                }
            }
        }
    } else if(strcmp(para, "getall") == 0){
        char getall_info[128];
        char n[16];
        for(int i = 0; i < MAX_USER; i++) {
            if (strcmp(pipes[i].name, value) == 0) {
                for (int j = 0; j < MAX_USER ; ++j) {
                    memset(getall_info, 0, 128);
                    strcpy(getall_info, "getall_");
                    strcat(getall_info, "_");
                    strcat(getall_info, pipes[j].name);
                    strcat(getall_info, "_");
                    sprintf(n, "%d", pipes[j].score[pipes[j].index]);
                    if(write(pipes[i].pipe[1], getall_info, strlen(getall_info)) == -1){
                        printf("Pipe is broken!\n");
                        continue;
                    }
                }
            }
        }
    }
    return 1;
}

int get_rand_char(char *info, int level){
    int n;
    char num[4];
    srand((unsigned)time(NULL));
    strcat(info, "[");
    strcat(info, "[");
    n = rand()%4;
    sprintf(num, "%d", n);
    strcat(info, num);
    for(int j = 1; j < level; j++){
        n = rand()%4;
        sprintf(num, "%d", n);
        strcat(info, ",");
        strcat(info, num);
    }
    strcat(info, "]");
    for(int i = 0; i < INFOR_SIZE; i++){
        strcat(info, ",[");
        n = rand()%4;
        sprintf(num, "%d", n);
        strcat(info, num);
        for(int j = 1; j < level; j++){
            n = rand()%4;
            sprintf(num, "%d", n);
            strcat(info, ",");
            strcat(info, num);
        }
        strcat(info, "]");
    }
    strcat(info, "]");
    return 1;
}