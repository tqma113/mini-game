//
// Created by tianqi on 17-12-23.
//

#include "websocket_session_init.h"

int deal_ws_request(int new_fd, struct sockaddr_in *client_addr){
    char recv_buf[HEAD_MAX_SIZE + 1] = "";
    char send_buf[HEAD_MAX_SIZE + 1] = "";

    int read_byte = 0;
    int file_byte = 0;

    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(new_fd, &read_set);

    struct timeval timeout;
    timeout.tv_sec = TIME_OUT_SEC;
    timeout.tv_usec = TIME_OUT_USEC;

    struct ws_request *req
            = (struct ws_request *)malloc(sizeof(struct ws_request));
    strcpy(req->address, inet_ntoa(client_addr->sin_addr));
    req->port = client_addr->sin_port;
    req->client_socket = new_fd;
    req->method_right_state = false;

    int resel = 0;
    int maxfd = new_fd+1;

    while(true){
        resel = select(maxfd, &read_set, NULL, NULL, &timeout);
        switch (resel) {
            case -1:
                perror("Cannot select read_fd!");
                return -1;
            case 0:
                printf("Listen time out!\n");
                return 1;
            default:
                if(FD_ISSET(new_fd, &read_set)){
                    read_byte = (int)recv(new_fd, recv_buf, 16384, 0);
                    if(read_byte == 0){
                        return 0;     //connect interrupt!
                    } else if(read_byte < 0) {
                        perror("Read request failure!");
                        return -1;
                    } else {
                        printf("%s", recv_buf);
                        return 1;
                    }
                } else {
                    perror("FD set happen crash!");
                    continue;
                }
        }
    }
}


int is_ws_request(char *request, struct ws_request* req){

}

int is_right_ws_method(bool *meth, char *method){

}

int is_right_ws_protocol(char *protocol){

}