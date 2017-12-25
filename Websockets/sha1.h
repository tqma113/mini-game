//
// Created by tianqi on 17-12-25.
//

#ifndef GAME_SHA1_H
#define GAME_SHA1_H

#include "../data_struct.h"


typedef struct SHA1Context{
    unsigned Message_Digest[5];
    unsigned Length_Low;
    unsigned Length_High;
    unsigned char Message_Block[64];
    int Message_Block_Index;
    int Computed;
    int Corrupted;
} SHA1Context;

#define SHA1CircularShift(bits,word) ((((word) << (bits)) & 0xFFFFFFFF) | ((word) >> (32-(bits))))

void SHA1ProcessMessageBlock(SHA1Context *context);

void SHA1Reset(SHA1Context *context);

void SHA1PadMessage(SHA1Context *context);

int SHA1Result(SHA1Context *context);

void SHA1Input(SHA1Context *context,const char *message_array,unsigned length);

char * sha1_hash(const char *source);

int tolower(int c);

int htoi(const char s[], int start, int len);

#endif //GAME_SHA1_H
