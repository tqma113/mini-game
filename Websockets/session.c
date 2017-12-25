//
// Created by tianqi on 17-12-25.
//

#include "session.h"

int webSocket_send(int fd, unsigned char *data, unsigned int dataLen, bool mod, Websocket_CommunicationType type) {
    unsigned char *webSocketPackage;
    unsigned int retLen, ret;
    unsigned int i;

    //printf("webSocket_send : %d\r\n", dataLen);
    //---------- websocket数据打包 ----------
    webSocketPackage = (unsigned char *)calloc(1, sizeof(char)*(dataLen + 128));  memset(webSocketPackage, 0, (dataLen + 128));
    retLen = (unsigned int)webSocket_enPackage(data, dataLen, webSocketPackage, (dataLen + 128), mod, type);
    printf("webSocket_send : %d\r\n" , retLen);
    for(i = 0; i < retLen; i ++)  printf("%.2X ", webSocketPackage[i]);
    printf("\r\n");
    ret = (unsigned int)send(fd, webSocketPackage, retLen, MSG_NOSIGNAL);
//    free(webSocketPackage);
//    return ret;
    return 1;
}

int webSocket_recv(int fd, unsigned char *data, unsigned int dataMaxLen) {
    unsigned char *webSocketPackage, *recvBuf;
    int ret, ret2 = 0;
    unsigned int retLen = 0;
    //int i;

    recvBuf = (unsigned char *)calloc(1, sizeof(char)*dataMaxLen);  memset(recvBuf, 0, dataMaxLen);
    ret = (unsigned int)recv(fd, recvBuf, dataMaxLen, MSG_NOSIGNAL);
    if(ret > 0)
    {
        //------------------------------------------------------------------- 和客户端的连接操作
//        if(strncmp(recvBuf, "GET", 3) == 0)
//        {
//            ret2 = webSocket_serverLinkToClient(fd, recvBuf, ret);
//            free(recvBuf);
//            if(ret2 < 0)
//            {
//                memset(data, 0, dataMaxLen);
//                strcpy(data, "connect false !\r\n");
//                return strlen("connect false !\r\n");
//            }
//            memset(data, 0, dataMaxLen);
//            strcpy(data, "connect ...\r\n");
//            return strlen("connect ...\r\n");
//        }

        //------------------------------------------------------------------- 正常数据交换

        //printf("webSocket_recv : %d\r\n", ret);
        //---------- websocket数据打包 ----------
        webSocketPackage = (unsigned char *)calloc(1, sizeof(char)*(ret + 128));  memset(webSocketPackage, 0, (ret + 128));
        ret2 = webSocket_dePackage(recvBuf, (unsigned int)ret, webSocketPackage, (unsigned int)(ret + 128), &retLen);
        if(ret2 == WCT_PING && retLen > 0)      // 解析为ping包, 自动回pong
        {
            webSocket_send(fd, webSocketPackage, retLen, true, WCT_PONG);
            // 显示数据
            printf("webSocket_recv : PING %d\r\n%s\r\n" , retLen, webSocketPackage);
            free(recvBuf);
            free(webSocketPackage);
            return WCT_NULL;
        }
        else if(retLen > 0 && (ret2 == WCT_TXTDATA || ret2 == WCT_BINDATA || ret2 == WCT_MINDATA))  // 解析为数据包
        {
            memcpy(data, webSocketPackage, retLen);     // 把解析得到的数据复制出去
            /*
            // 显示数据
            if(webSocketPackage[0] >= ' ' && webSocketPackage[0] <= '~')
            {
                printf("\r\nwebSocket_recv : New Package StrFile ret2:%d/retLen:%d\r\n%s\r\n" , ret2, retLen, webSocketPackage);
            }
            else
            {
                printf("\r\nwebSocket_recv : New Package BinFile ret2:%d/retLen:%d\r\n" , ret2, retLen);
                for(i = 0; i < retLen; i++) printf("%.2X ", webSocketPackage[i]); printf("\r\n");
            }*/
            free(recvBuf);
            free(webSocketPackage);
            return retLen;
        }/*
        else
        {
            // 显示数据
            if(recvBuf[0] >= ' ' && recvBuf[0] <= '~')
                printf("\r\nwebSocket_recv : ret:%d/ret2:%d/retLen:%d\r\n%s\r\n" , ret, ret2, retLen, recvBuf);
            else
            {
                printf("\r\nwebSocket_recv : ret:%d/ret2:%d/retLen:%d\r\n%s\r\n" , ret, ret2, retLen, recvBuf);
                for(i = 0; i < ret; i++) printf("%.2X ", recvBuf[i]); printf("\r\n");
            }
        }*/
        free(recvBuf);
        free(webSocketPackage);
        return -ret;
    }
    else
    {
        free(recvBuf);
        return ret;
    }
}

int webSocket_enPackage(
        unsigned char *data,
        unsigned int dataLen,
        unsigned char *package,
        unsigned int packageMaxLen,
        bool isMask,
        Websocket_CommunicationType type) {
    unsigned char maskKey[4] = {0};    // 掩码
    unsigned char temp1, temp2;
    int count;
    unsigned int i, len = 0;

    if(packageMaxLen < 2)
        return -1;

    if(type == WCT_MINDATA)
        *package++ = 0x00;
    else if(type == WCT_TXTDATA)
        *package++ = 0x81;
    else if(type == WCT_BINDATA)
        *package++ = 0x82;
    else if(type == WCT_DISCONN)
        *package++ = 0x88;
    else if(type == WCT_PING)
        *package++ = 0x89;
    else if(type == WCT_PONG)
        *package++ = 0x8A;
    else
        return -1;
    //
    if(isMask)
        *package = 0x80;
    len += 1;
    //
    if(dataLen < 126)
    {
        *package++ |= (dataLen&0x7F);
        len += 1;
    }
    else if(dataLen < 65536)
    {
        if(packageMaxLen < 4)
            return -1;
        *package++ |= 0x7E;
        *package++ = (char)((dataLen >> 8) & 0xFF);
        *package++ = (unsigned char)((dataLen >> 0) & 0xFF);
        len += 3;
    }
    else if(dataLen < 0xFFFFFFFF)
    {
        if(packageMaxLen < 10)
            return -1;
        *package++ |= 0x7F;
        *package++ = 0; //(char)((dataLen >> 56) & 0xFF);   // 数据长度变量是 unsigned int dataLen, 暂时没有那么多数据
        *package++ = 0; //(char)((dataLen >> 48) & 0xFF);
        *package++ = 0; //(char)((dataLen >> 40) & 0xFF);
        *package++ = 0; //(char)((dataLen >> 32) & 0xFF);
        *package++ = (char)((dataLen >> 24) & 0xFF);        // 到这里就够传4GB数据了
        *package++ = (char)((dataLen >> 16) & 0xFF);
        *package++ = (char)((dataLen >> 8) & 0xFF);
        *package++ = (char)((dataLen >> 0) & 0xFF);
        len += 9;
    }
    //
    if(isMask)    // 数据使用掩码时, 使用异或解码, maskKey[4]依次和数据异或运算, 逻辑如下
    {
        if(packageMaxLen < len + dataLen + 4)
            return -1;
        webSocket_getRandomString(maskKey, sizeof(maskKey));    // 随机生成掩码
        *package++ = maskKey[0];
        *package++ = maskKey[1];
        *package++ = maskKey[2];
        *package++ = maskKey[3];
        len += 4;
        for(i = 0, count = 0; i < dataLen; i++)
        {
            temp1 = maskKey[count];
            temp2 = data[i];
            *package++ = (char)(((~temp1)&temp2) | (temp1&(~temp2)));  // 异或运算后得到数据
            count += 1;
            if(count >= sizeof(maskKey))    // maskKey[4]循环使用
                count = 0;
        }
        len += i;
        *package = '\0';
    }
    else    // 数据没使用掩码, 直接复制数据段
    {
        if(packageMaxLen < len + dataLen)
            return -1;
        memcpy(package, data, dataLen);
        package[dataLen] = '\0';
        len += dataLen;
    }
    //
    return len;
}

int webSocket_dePackage(
        unsigned char *data,
        unsigned int dataLen,
        unsigned char *package,
        unsigned int packageMaxLen,
        unsigned int *packageLen) {
    unsigned char maskKey[4] = {0};    // 掩码
    unsigned char temp1, temp2;
    char Mask = 0, type;
    int count, ret;
    unsigned int i, len = 0, dataStart = 2;
    if(dataLen < 2)
        return -1;

    type = data[0]&0x0F;

    if((data[0]&0x80) == 0x80)
    {
        if(type == 0x01)
            ret = WCT_TXTDATA;
        else if(type == 0x02)
            ret = WCT_BINDATA;
        else if(type == 0x08)
            ret = WCT_DISCONN;
        else if(type == 0x09)
            ret = WCT_PING;
        else if(type == 0x0A)
            ret = WCT_PONG;
        else
            return WCT_ERR;
    }
    else if(type == 0x00)
        ret = WCT_MINDATA;
    else
        return WCT_ERR;
    //
    if((data[1] & 0x80) == 0x80)
    {
        Mask = 1;
        count = 4;
    }
    else
    {
        Mask = 0;
        count = 0;
    }
    //
    len = data[1] & 0x7F;
    //
    if(len == 126)
    {
        if(dataLen < 4)
            return WCT_ERR;
        len = data[2];
        len = (len << 8) + data[3];
        if(dataLen < len + 4 + count)
            return WCT_ERR;
        if(Mask)
        {
            maskKey[0] = data[4];
            maskKey[1] = data[5];
            maskKey[2] = data[6];
            maskKey[3] = data[7];
            dataStart = 8;
        }
        else
            dataStart = 4;
    }
    else if(len == 127)
    {
        if(dataLen < 10)
            return WCT_ERR;
        if(data[2] != 0 || data[3] != 0 || data[4] != 0 || data[5] != 0)    // 使用8个字节存储长度时, 前4位必须为0, 装不下那么多数据...
            return WCT_ERR;
        len = data[6];
        len = (len << 8) + data[7];
        len = (len << 8) + data[8];
        len = (len << 8) + data[9];
        if(dataLen < len + 10 + count)
            return WCT_ERR;
        if(Mask)
        {
            maskKey[0] = data[10];
            maskKey[1] = data[11];
            maskKey[2] = data[12];
            maskKey[3] = data[13];
            dataStart = 14;
        }
        else
            dataStart = 10;
    }
    else
    {
        if(dataLen < len + 2 + count)
            return WCT_ERR;
        if(Mask)
        {
            maskKey[0] = data[2];
            maskKey[1] = data[3];
            maskKey[2] = data[4];
            maskKey[3] = data[5];
            dataStart = 6;
        }
        else
            dataStart = 2;
    }
    //
    if(dataLen < len + dataStart)
        return WCT_ERR;
    //
    if(packageMaxLen < len + 1)
        return WCT_ERR;
    //
    if(Mask)    // 解包数据使用掩码时, 使用异或解码, maskKey[4]依次和数据异或运算, 逻辑如下
    {
        //printf("depackage : len/%d\r\n", len);
        for(i = 0, count = 0; i < len; i++)
        {
            temp1 = maskKey[count];
            temp2 = data[i + dataStart];
            *package++ =  (char)(((~temp1)&temp2) | (temp1&(~temp2)));  // 异或运算后得到数据
            count += 1;
            if(count >= sizeof(maskKey))    // maskKey[4]循环使用
                count = 0;
            //printf("%.2X|%.2X|%.2X, ", temp1, temp2, *(package-1));
        }
        *package = '\0';
    }
    else    // 解包数据没使用掩码, 直接复制数据段
    {
        memcpy(package, &data[dataStart], len);
        package[len] = '\0';
    }
    *packageLen = len;
    //
    return ret;
}

void webSocket_getRandomString(unsigned char *buf, unsigned int len) {
    unsigned int i;
    unsigned char temp;
    srand((int)time(0));
    for(i = 0; i < len; i++)
    {
        temp = (unsigned char)(rand()%256);
        if(temp == 0)   // 随机数不要0, 0 会干扰对字符串长度的判断
            temp = 128;
        buf[i] = temp;
    }
}