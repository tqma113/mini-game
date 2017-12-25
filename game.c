#include <sys/wait.h>
#include "game.h"

int main() {
    int webserver_fd = 0;
    int websocket_fd = 0;
    int game_fd = 0;

    int game_pipe[2];
    int webSocket_pipe[2];
    pipe(game_pipe);
    pipe(webSocket_pipe);

    if((game_fd = fork()) == 0){
        if(start_game(game_pipe[1], webSocket_pipe[0]) == -1){
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

    if((websocket_fd = fork()) == 0){
        if(start_websocket(WEBSOCKET_PORT, webSocket_pipe[1], game_pipe[0]) == -1){
            printf("Websocket server start failure!\n");
            exit(EXIT_FAILURE);
        }
    }
    wait(NULL);
    printf("Some one server has exited!\n");
    kill(0, SIGTERM);
    printf("Success!\n");
}