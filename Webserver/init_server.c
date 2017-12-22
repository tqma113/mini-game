//
// Created by tianqi on 17-12-22.
//

#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdbool.h>
#include "init_server.h"

int init_server(int *server_fd, struct sockaddr_in *server, int port){
    if((*server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Server create falure...\n");
        return -1;
    }
    /*
        int setsockopt(
        SOCKET s,
        int level,
        int optname,
        const char* optval,
        int optlen
        );
        s(套接字): 指向一个打开的套接口描述字
        level:(级别)： 指定选项代码的类型。
        SOL_SOCKET: 基本套接口
        IPPROTO_IP: IPv4套接口
        IPPROTO_IPV6: IPv6套接口
        IPPROTO_TCP: TCP套接口
        optname(选项名)： 选项名称
        optval(选项值): 是一个指向变量的指针 类型：整形，套接口结构， 其他结构类型:linger{}, timeval{ }
        optlen(选项长度) ：optval 的大小

        返回值：标志打开或关闭某个特征的二进制选项
    */
    // bool bDontLinger = true;
    // if(setsockopt(*server_fd, IPPROTO_IP, SO_LINGER, (const char*)&bDontLinger,sizeof(bool)) == -1){  //没有socket断开后waiting时间
    //     perror("Set scoket option error! SO_DONTLINGER");
    //     return -1;
    // }
    // int nNetTimeout = 1000;
    // if(setsockopt(*server_fd, IPPROTO_IP, SO_SNDTIMEO, (char *)&nNetTimeout,sizeof(int)) == -1){   //设置发送时间
    //     perror("Set scoket option error! SO_SNDTIMEO");
    //     return -1;
    // }
    // if(setsockopt(*server_fd, IPPROTO_IP, SO_RCVTIMEO, (char *)&nNetTimeout,sizeof(int)) == -1){    //设置接受时间
    //     perror("Set scoket option error! SO_RCVTIMEO");
    //     return -1;
    // }


    int reuse = 1;
    if(setsockopt(*server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse))){
        perror("Time set error!");
        return -1;
    }
    server->sin_family=AF_INET;
    server->sin_port=htons(port);
    server->sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(*server_fd, (const struct sockaddr *)server, sizeof(struct sockaddr_in)) < 0){       //端口绑定
        printf("Port has been used!\n");
        return -1;
    }
    if(listen(*server_fd, PACKAGE_SIZE)==-1){                                            //监听端口
        perror("Listening server error!\n");
        return -1;
    }
    printf("Server start...\n");
    return 0;
}
