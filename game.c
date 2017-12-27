#include <sys/wait.h>
#include "game.h"

int main() {
    int webserver_fd = 0;
    int game_fd = 0;

//    int game_pipe[2];
//    int webSocket_pipe[2];
//    pipe(game_pipe);
//    pipe(webSocket_pipe);

    if((game_fd = fork()) == 0){
        if(start_game() == -1){
            printf("Game server start failure!\n");
            exit(EXIT_FAILURE);
        }
    }

    if((webserver_fd = fork()) == 0){
        if(start_web(WEBSERVER_PORT) == -1){
            printf("Webserver start failure!\n");
            exit(EXIT_FAILURE);
        }
    }

    wait(NULL);
    printf("Some one server has exited!\n");
    kill(0, SIGTERM);
    printf("Success!\n");
}