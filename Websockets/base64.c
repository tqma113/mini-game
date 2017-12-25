//
// Created by tianqi on 17-12-24.
//

#include "base64.h"
//#include <openssl/sha.h>
//#include <openssl/pem.h>
//#include <openssl/bio.h>
//#include <openssl/evp.h>
//
//int base64_encode(char *in_str, int in_len, char *out_str)
//{
//    BIO *b64, *bio;
//    BUF_MEM *bptr = NULL;
//    size_t size = 0;
//
//    if (in_str == NULL || out_str == NULL)
//        return -1;
//
//    b64 = BIO_new(BIO_f_base64());
//    bio = BIO_new(BIO_s_mem());
//    bio = BIO_push(b64, bio);
//
//    BIO_write(bio, in_str, in_len);
//    BIO_flush(bio);
//
//    BIO_get_mem_ptr(bio, &bptr);
//    memcpy(out_str, bptr->data, bptr->length);
//    out_str[bptr->length-1] = '\0';
//    size = bptr->length;
//
//    BIO_free_all(bio);
//    return size;
//}

int base64_encode( const unsigned char *bindata, char *base64, int binlength){

    const char base64str[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int i, j;
    unsigned char current;
    for ( i = 0, j = 0 ; i < binlength ; i += 3 )
    {
        current = (bindata[i] >> 2) ;
        current &= (unsigned char)0x3F;
        base64[j++] = base64str[(int)current];
        current = ( (unsigned char)(bindata[i] << 4 ) ) & ( (unsigned char)0x30 ) ;
        if ( i + 1 >= binlength )
        {
            base64[j++] = base64str[(int)current];
            base64[j++] = '=';
            base64[j++] = '=';
            break;
        }
        current |= ( (unsigned char)(bindata[i+1] >> 4) ) & ( (unsigned char) 0x0F );
        base64[j++] = base64str[(int)current];
        current = ( (unsigned char)(bindata[i+1] << 2) ) & ( (unsigned char)0x3C ) ;
        if ( i + 2 >= binlength )
        {
            base64[j++] = base64str[(int)current];
            base64[j++] = '=';
            break;
        }
        current |= ( (unsigned char)(bindata[i+2] >> 6) ) & ( (unsigned char) 0x03 );
        base64[j++] = base64str[(int)current];
        current = ( (unsigned char)bindata[i+2] ) & ( (unsigned char)0x3F ) ;
        base64[j++] = base64str[(int)current];
    }
    base64[j] = '\0';
    return j;
}

//unsigned char *base64_encode(unsigned char *str) {
//    long len;
//    long str_len;
//    unsigned char *res;
//    int i,j;
//    //定义base64编码表
//    unsigned char *base64_table="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//
//    //计算经过base64编码后的字符串长度
//    str_len=strlen(str);
//    if(str_len % 3 == 0)
//    len=str_len/3*4;
//    else
//    len=(str_len/3+1)*4;
//
//    res=malloc(sizeof(unsigned char)*len+1);
//    res[len]='\0';
//
//    //以3个8位字符为一组进行编码
//    for(i=0,j=0;i<len-2;j+=3,i+=4)
//    {
//        res[i]=base64_table[str[j]>>2]; //取出第一个字符的前6位并找出对应的结果字符
//        res[i+1]=base64_table[(str[j]&0x3)<<4 | (str[j+1]>>4)]; //将第一个字符的后位与第二个字符的前4位进行组合并找到对应的结果字符
//        res[i+2]=base64_table[(str[j+1]&0xf)<<2 | (str[j+2]>>6)]; //将第二个字符的后4位与第三个字符的前2位组合并找出对应的结果字符
//        res[i+3]=base64_table[str[j+2]&0x3f]; //取出第三个字符的后6位并找出结果字符
//    }
//
//    switch(str_len % 3)
//    {
//        case 1:
//        res[i-2]='=';
//        res[i-1]='=';
//        break;
//        case 2:
//        res[i-1]='=';
//        break;
//    }
//
//    return res;
//}
//
//unsigned char *base64_decode(unsigned char *code) {
//    //根据base64表，以字符找到对应的十进制数据
//    int table[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,62,0,0,0,63,52,53,54,55,56,57,58,59,60,61,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,0,0,0,0,0,0,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51};
//    long len;
//    long str_len;
//    unsigned char *res;
//    int i,j;
//
//    //计算解码后的字符串长度
//    len=strlen(code);
//    //判断编码后的字符串后是否有=
//    if(strstr(code,"=="))
//    str_len=len/4*3-2;
//    else if(strstr(code,"="))
//    str_len=len/4*3-1;
//    else
//    str_len=len/4;
//
//    res=malloc(sizeof(unsigned char)*str_len+1);
//    res[str_len]='\0';
//
//    //以4个字符为一位进行解码
//    for(i=0,j=0;i < len-2;j+=3,i+=4)
//    {
//        res[j]=((unsigned char)table[code[i]])<<2 | (((unsigned char)table[code[i+1]])>>4); //取出第一个字符对应base64表的十进制数的前6位与第二个字符对应base64表的十进制数的后2位进行组合
//        res[j+1]=(((unsigned char)table[code[i+1]])<<4) | (((unsigned char)table[code[i+2]])>>2); //取出第二个字符对应base64表的十进制数的后4位与第三个字符对应bas464表的十进制数的后4位进行组合
//        res[j+2]=(((unsigned char)table[code[i+2]])<<6) | ((unsigned char)table[code[i+3]]); //取出第三个字符对应base64表的十进制数的后2位与第4个字符进行组合
//    }
//
//    return res;
//
//}