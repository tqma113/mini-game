//
// Created by tianqi on 17-12-22.
//

#include "init_websocket_server.h"
#include "init_ws_log.h"
#include "websocket_session_init.h"
#include "websocket_server.h"
#include "session.h"

int start_websocket(int port, int shm_id, int write_fd){

    struct PIPE *pipes;
    printf("%d", shm_id);
    pipes = (struct PIPE *)shmat(shm_id, NULL, 0);
    if(pipes == -1){
        printf("Cannot link sharing memory to this process!(Game server)\n");
        return -1;
    }

    int server_fd = 0;
    int new_fd = 0;
    FILE *log_fd;
    pid_t client_fd = 0;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t socketaddr_in_size = 0;
    if(init_ws_server(&server_fd, &server_addr, port) == -1){
        perror("Server start error!");
        return -1;
    }
    if(init_ws_log(&log_fd) == -1){
        perror("Log process cannot run!");
        return -1;
    }
    socketaddr_in_size = sizeof(struct sockaddr_in);
    printf("Websocket server start success!\n");
    while(true){
        int i = 0;
        if((new_fd = accept(server_fd, (struct sockaddr *) &client_addr, &socketaddr_in_size)) == -1){
            perror("Cannot accpet request!");
            printf("pid %d loss connection to %s\n", getpid(), inet_ntoa(client_addr.sin_addr));
            close(server_fd);
            return -1;
        }
        if((client_fd = fork()) > 0){
            close(new_fd);
            continue;
        } else if(client_fd == 0){
            for(i = 0; i < 20; i++){

                if(pipes[i].useState == false){
                    pipe(pipes[i].pipe);
                    pipes[i].useState = true;
                    break;
                }
            }
            if(i < MAX_USER) {
                printf("\npid %d process Websocket session from %s : %d\n", getpid(), inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port));
                fprintf(log_fd, "pid %d process Websocket session from %s : %d\n", getpid(), inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port));
                printf("Start deal request %d of %s:%d\n", new_fd, inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port));
                printf("Pipe: %d\n", pipes[i].pipe[1]);
                switch (deal_ws_request(new_fd, &client_addr, pipes[i].pipe[0], write_fd)){
                    case -1:
                        perror("Deal request failure!");
                        break;
                    case 1:
                        break;
                    default:
                        printf("Unknow error!\n");
                        break;
                }
                pipes[i].useState = false;
                printf("pid %d close Websocket connection to %s\n", getpid(), inet_ntoa(client_addr.sin_addr));
                fprintf(log_fd, "pid %d close Websocket connection to %s\n", getpid(), inet_ntoa(client_addr.sin_addr));
                close(new_fd);
            }else {
                //close(server_fd);
                webSocket_send(new_fd, (unsigned char *)"The number of users is full!", (unsigned int)strlen("The number of users is full!"), false,WCT_TXTDATA);
                close(new_fd);
            }

        } else {
            perror("Cannot create dealing process!");
            break;
        }
    }

    close(server_fd);
    return 1;
}