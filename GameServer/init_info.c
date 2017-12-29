//
// Created by tianqi on 17-12-25.
//

#include "init_info.h"

int init_info(int *game_info){
    int file_fd;
    char num[4];
    FILE * fp;
    char file[] = "/home/tianqi/CLionProjects/Game/init_infor.txt";
    if((file_fd = open(file, O_RDONLY) == -1)){
        printf("Game server init file cannot been fond!\n");
        return -1;
    }
//    if((fp = fopen(file, "w")) == NULL){
//        printf("Game server init file cannot been fond!:%s\n" , file);
//        return -1;
//    }
//    printf("Start init infor file\n");
//    if(read(file_fd, num ,2) <= 0){
//        printf("Game server init file cannot been read!\n");
//        fflush(stdout);
//        exit(1);
//    }
//    printf("finish init infor file\n");
//    printf("%s", num);
//    int index = 0;
//    while(fscanf(fp,"%d ",&game_info[index]))
//    {
//        if (feof(fp))
//        {
//            break;
//        }
//        index++;
//    }
//
//    int level = 0;
//    int i;
//    char n[8];
//    while((level = readline(num, level,n)) != -1){
//        sprintf(n ,"%d", game_info[i]);
//    }
    return 1;
}


int readline(char* allbuf,int level,char* linebuf){
    int len = strlen(allbuf);
    for (;level<len;++level)
    {
        if(allbuf[level]=='\r' && allbuf[level+1]=='\n')
            return level+2;
        else
            *(linebuf++) = allbuf[level];
    }
    return -1;
}