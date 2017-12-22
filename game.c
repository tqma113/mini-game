#include <stdio.h>
#include <signal.h>

#include "Webserver/webserver.h"
#include "Websockets/websocket_server.h"
#include "game.h"

int main() {
    int webserver_fd = 0;
    int websocket_fd = 0;

    if((webserver_fd = fork()) == 0){
        if(start_web(WEBSERVER_PORT) == -1){
            printf("Webserver start failure!\n");
            exit(EXIT_FAILURE);
        }
    }

    if((websocket_fd = fork()) == 0){
        if(start_websocket(WEBSOCKET_PORT) == -1){
            printf("Websocket server start failure!");
            exit(EXIT_FAILURE);
        }
    }

    sleep(10000);
    kill(0, SIGTERM);
    printf("Success!\n");
}