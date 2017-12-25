//
// Created by tianqi on 17-12-24.
//

#ifndef GAME_BASE64_H
#define GAME_BASE64_H

#include "../data_struct.h"
//
//int base64_encode(char *in_str, int in_len, char *out_str);


int base64_encode( const unsigned char *bindata, char *base64, int binlength);

//unsigned char *base64_encode(unsigned char *str);
//
//unsigned char *bae64_decode(unsigned char *code);

#endif //GAME_BASE64_H
