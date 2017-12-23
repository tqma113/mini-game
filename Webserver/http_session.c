//
// Created by tianqi on 17-12-21.
//

#include "http_session.h"

int deal_request(int new_fd, struct sockaddr_in *client_addr){

    char recv_buf[READ_BUF_SIZE + 1] = "";
    char file_buf[FILE_BUF_SIZE + 1] = "";
    char send_buf[SEND_BUF_SIZE + 1] = "";
    char dir[URI_MAX_SIZE] = "";
    int read_byte = 0;
    int file_byte = 0;

    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(new_fd, &read_set);

    struct timeval timeout;
    timeout.tv_sec = TIME_OUT_SEC;
    timeout.tv_usec = TIME_OUT_USEC;

    struct http_request *req
            = (struct http_request *)malloc(sizeof(struct http_request));
    strcpy(req->address, inet_ntoa(client_addr->sin_addr));
    req->port = client_addr->sin_port;
    req->client_socket = new_fd;

    int resel = 0;
    int maxfd = new_fd+1;

    while (true){
        resel = select(maxfd, &read_set, NULL, NULL, &timeout);
        switch (resel) {
            case -1:
                perror("Cannot select read_fd!");
                return -1;
            case 0:
                printf("Listen time out!\n");
                return 1;
                break;
            default:
                if(FD_ISSET(new_fd, &read_set)){
                    read_byte = (int)recv(new_fd, recv_buf, 16384, 0);
                    if(read_byte == 0){
                        return 0;     //connect interrupt!
                    } else if(read_byte < 0){
                        perror("Read request failure!");
                        return -1;
                    } else {
                        if(is_http_request(recv_buf, req) == -1){
                            printf("Request is illegal!");
                            send_error_msg(req->state, send_buf);
                            send(new_fd, send_buf, SEND_BUF_SIZE, 0);
                            return -1;
                        } else {
                            div_type(req);
                            switch (req->f_type){
                                case ROOT:
                                    if(get_dir_info(file_buf, ROOT_DIR, req) == -1){
                                        printf("Get dir info failed!\n");
                                        send_error_msg(req->state, send_buf);
                                        send(new_fd, send_buf, SEND_BUF_SIZE, 0);
                                        return -1;
                                    }
                                    if(send_normal_msg(req, send_buf, sizeof(file_buf))){
                                        printf("Sending message failed!\n");
                                        return -1;
                                    }
                                    strcat(send_buf, file_buf);
                                    break;
                                case NDIR:
                                    strcpy(dir, ROOT_DIR);
                                    strcat(dir, req->file);
                                    if(get_dir_info(file_buf, dir, req) == -1){
                                        printf("Get dir info failed!\n");
                                        send_error_msg(req->state, send_buf);
                                        send(new_fd, send_buf, SEND_BUF_SIZE, 0);
                                        return -1;
                                    }
                                    printf("%s", file_buf);
                                    if(send_normal_msg(req, send_buf, sizeof(file_buf))){
                                        printf("Sending message failed!\n");
                                        return -1;
                                    }
                                    strcat(send_buf, file_buf);
                                    break;
                                case NFILE:
                                    if(is_right_type(&(req->m_type), req->file) == -1){
                                        req->state = 415;
                                        printf("HTTP request file has wrong type!");
                                        send_error_msg(req->state, send_buf);
                                        send(new_fd, send_buf, SEND_BUF_SIZE, 0);
                                        return -1;
                                    }
                                    if((file_byte = access_file(req, file_buf)) == -1){
                                        printf("File:%s cannot been access!", req->file);
                                        send_error_msg(req->state, send_buf);
                                        send(new_fd, send_buf, SEND_BUF_SIZE, 0);
                                        return -1;
                                    }
                                    if(send_normal_msg(req, send_buf, file_byte) == -1){
                                        printf("Sending message failed!\n");
                                        return -1;
                                    }
                                    memcpy(send_buf + strlen(send_buf), file_buf, sizeof(file_buf));
                                    break;
                                default:
                                    printf("Deal request has failed!\n");
                                    return -1;
                                    break;
                            }
                            send(new_fd, send_buf, SEND_BUF_SIZE, 0);
                            return 1;
                        }
                    }
                } else {
                    perror("FD set happen crash!");
                    continue;
                }
        }
    }
}

int is_http_request(char *request, struct http_request* req){
    char me[8] = "";
    char uri[128] = "";

    sscanf(request,"%s %s %s",
           me, uri, req->protocol);

    if(strcmp(uri, "/") != 0){
        strcpy(req->uri, uri);
        int len = (int)strlen(req->uri);
        int base = 0;
        while( ( (req->uri[base] != '\0') &&  (req->uri[base] != '?')) && base < len)
        {
            base++;
        }
        strncpy(req->file, req->uri, (size_t)base);
    } else {
        strcpy(req->uri, CATALOGUE);
        strcpy(req->file, "");
    }

    if(is_right_method(&(req->method), me) == -1){
        req->state = 405;
        printf("HTTP request file has wrong method!");
        return -1;
    }

    if(is_right_protocol(req->protocol) == -1){
        req->state = 505;
        printf("HTTP request file has wrong protocol!");
        return -1;
    }

    return 1;
}

int is_right_method(enum METHOD *meth, char *method){
    if(strcmp(method,"GET") == 0){
        *meth = GET;
    } else if(strcmp(method,"POST") == 0){
        *meth = POST;
    } else if(strcmp(method, "PUT") == 0){
        *meth = PUT;
    } else if(strcmp(method, "DELETE") == 0){
        *meth = DELETE;
    } else if(strcmp(method, "HEAD") == 0){
        *meth = HEAD;
    } else if(strcmp(method, "OPTIONS") == 0){
        *meth = OPTIONS;
    } else if(strcmp(method, "TRACE") == 0){
        *meth = TRACE;
    } else if(strcmp(method, "CONNECT") == 0){
        *meth = CONNECT;
    } else {
        return -1;
    }
    return 0;
}

int is_right_protocol(char *protocol){
    if(strcmp(protocol, "HTTP/1.1") == 0)    /* HTTP Request firt line like this 'GET /index.html HTTP/1.1' , so last 10 byte are HTTP/1.1\r\n*/
    {
        return 1;
    } else {
        protocol = NULL;
        return -1;
    }
}

void div_type(struct http_request *req) {
    if(strcmp(req->uri, CATALOGUE) == 0){
        req->f_type = ROOT;
        return;
    }
    int len = 0;
    len = (int)strlen(req->file);
    int dot = len - 1;
    while( dot >= 0 && req->file[dot] != '.'){
        dot--;
    }
    if(dot <= 0){
        req->f_type = NDIR;
    } else {
        req->f_type = NFILE;
    }
}

int is_right_type(enum MIMETYPE *filetype, char *filename){
    int len = 0;
    len = (int)strlen(filename);
    int dot = len - 1;
    while( dot >= 0 && filename[dot] != '.')
    {
        dot--;
    }
    if(dot < 0)            /* the uri is '/',so default type text/html returns */
    {
        *filetype = TXT;
        return 0;
    }
    dot++;
    char *type_off;
    type_off = filename + dot;
    if(!strcmp(type_off, "html") || !strcmp(type_off, "HTML")){
        *filetype = HTML;
    }
    if(!strcmp(type_off, "css") || !strcmp(type_off, "CSS")){
        *filetype = CSS;
    }
    if(!strcmp(type_off, "js") || !strcmp(type_off, "JS")){
        *filetype = JS;
    }
    if(!strcmp(type_off, "jpg") || !strcmp(type_off, "JPG")){
        *filetype = JPG;
    }
    if(!strcmp(type_off, "png") || !strcmp(type_off, "PNG")){
        *filetype = PNG;
    }
    if(!strcmp(type_off, "avi") || !strcmp(type_off, "AVI")){
        *filetype = AVI;
    }
    if(!strcmp(type_off, "mp4") || !strcmp(type_off, "MP4")){
        *filetype = MP4;
    }
    if(!strcmp(type_off, "mp3") || !strcmp(type_off, "MP3")){
        *filetype = MP3;
    }
    if(!strcmp(type_off, "gif") || !strcmp(type_off, "GIF")){
        *filetype = GIF;
    }
    if(!strcmp(type_off, "txt") || !strcmp(type_off, "TXT")){
        *filetype = TXT;
    }
    return 0;
}

int access_file(struct http_request *req, char *file_buf){
    int read_count = 0;
    char filename[URI_MAX_SIZE] = "";
    strcpy(filename, ROOT_DIR);
    strcat(filename, req->file);
    if(access(filename, 0) != 0){
        req->state = 404;
        printf("This file:%s cannot been found!\n", filename);
        return -1;
    }
    if(access(filename, 4) != 0){
        req->state = 403;
        printf("This file:%s cannot been read!\n", filename);
        return -1;
    }
    int fd = open(filename, O_RDONLY);
    if(fd == -1)
    {
        printf("This file:%s cannot been found!\n", filename);
        req->state = 404;
        return -1;
    }
    unsigned long st_size;
    struct stat st;
    if(fstat(fd, &st) == -1)
    {
        printf("The file:%s cannot been access!\n", filename);
        req->state = 403;
        close(fd);
        return -1;
    }
    st_size = (unsigned long)st.st_size;
    if(st_size > FILE_BUF_SIZE)
    {
        printf("The file %s is too large.\n", filename);
        req->state = 413;
        close(fd);
        return -1;
    }
    if((read_count = (int)read(fd, file_buf, FILE_BUF_SIZE)) == -1)
    {
        printf("The file:%s cannot been access!", req->file);
        req->state = 403;
        close(fd);
        return -1;
    }
    printf("file ok\n");
    printf("file %s size : %lu , read %d\n", req->file, st_size, read_count);
    close(fd);
    req->state = 200;
    return read_count;
}

char *weekday_to_str(int n){
    char w[8] = "", *ww;
    switch(n){
        case 0: strcat(w, "Sun"); break;
        case 1: strcat(w, "Mon"); break;
        case 2: strcat(w, "Tues"); break;
        case 3: strcat(w, "Wed"); break;
        case 4: strcat(w, "Thurs"); break;
        case 5: strcat(w, "Fri"); break;
        case 6: strcat(w, "Sat"); break;
        default: return NULL;
    }
    ww = w;
    return ww;
}

char *int_to_string(int n){
    char *num = NULL, nu[16] ="";
    memset(nu, '\0', sizeof(nu));
    sprintf(nu, "%d", n);
    num = nu;
    return num;
}

char *get_current_time(){
    char str[64] = "";
    char wday[8] = "";
    char *s;
    struct tm *ptr;
    time_t lt;
    lt = time(NULL);
    ptr = gmtime(&lt);
    strcpy(wday, weekday_to_str(ptr->tm_wday));
    strcat(str, wday);
    strcat(str, " ");
    strcat(str, int_to_string(ptr->tm_year + 1900));
    strcat(str, "/");
    strcat(str, int_to_string(ptr->tm_mon));
    strcat(str, "/");
    strcat(str, int_to_string(ptr->tm_mday));
    strcat(str, "/");
    strcat(str, int_to_string(ptr->tm_hour));
    strcat(str, ":");
    strcat(str, int_to_string(ptr->tm_min));
    strcat(str, ":");
    strcat(str, int_to_string(ptr->tm_sec));
    s = str;
    return s;
}

char *type_to_str(enum MIMETYPE type){
    char *ty = NULL, typ[64] = "";
    switch(type){
        case HTML: strcat(typ, "text/html"); break;
        case CSS: strcat(typ, "text/css"); break;
        case JS: strcat(typ, "text/javascript"); break;
        case JPG: strcat(typ, "image/jpg"); break;
        case PNG: strcat(typ, "image/png"); break;
        case AVI: strcat(typ, "video/avi"); break;
        case MP3: strcat(typ, "audio/mp3"); break;
        case MP4: strcat(typ, "video/mp4"); break;
        case TXT: strcat(typ, "text/txt"); break;
        default: strcat(typ, "error"); break;
    }
    ty = typ;
    return ty;
}

char *get_first_line(int state){
    char *head, h[32] = "";
    switch(state){
        case 100: strcat(h, "HTTP/1.1 100 Continue\r\n"); break;
        case 101: strcat(h, "HTTP/1.1 101 Switching Protocols\r\n"); break;
        case 200: strcat(h, "HTTP/1.1 200 OK\r\n");  break;
        case 201: strcat(h, "HTTP/1.1 201 Created\r\n"); break;
        case 202: strcat(h, "HTTP/1.1 202 Accepted\r\n"); break;
        case 203: strcat(h, "HTTP/1.1 203 Non-authoritative Information\\r\n"); break;
        case 204: strcat(h, "HTTP/1.1 204 No Content\r\n"); break;
        case 205: strcat(h, "HTTP/1.1 205 Reset Content\r\n"); break;
        case 206: strcat(h, "HTTP/1.1 206 Partial Content\r\n"); break;
        case 300: strcat(h, "HTTP/1.1 300 Multiple Choices\r\n"); break;
        case 301: strcat(h, "HTTP/1.1 301 Moved Permanently\r\n"); break;
        case 302: strcat(h, "HTTP/1.1 302 Found\r\n"); break;
        case 303: strcat(h, "HTTP/1.1 303 See Other\r\n"); break;
        case 304: strcat(h, "HTTP/1.1 304 Not MOdified\r\n"); break;
        case 305: strcat(h, "HTTP/1.1 305 Use Proxy\r\n"); break;
        case 306: strcat(h, "HTTP/1.1 306 Unused\r\n"); break;
        case 307: strcat(h, "HTTP/1.1 307 Temporary\r\n"); break;
        case 400: strcat(h, "HTTP/1.1 400 Bad Request\r\n"); break;
        case 401: strcat(h, "HTTP/1.1 401 Unauthorized\r\n"); break;
        case 402: strcat(h, "HTTP/1.1 402 Payment Required\r\n"); break;
        case 403: strcat(h, "HTTP/1.1 403 Forbidden\r\n"); break;
        case 404: strcat(h, "HTTP/1.1 404 Not Found\r\n"); break;
        case 405: strcat(h, "HTTP/1.1 405 Mehtod Not Allowed\r\n"); break;
        case 406: strcat(h, "HTTP/1.1 406 Not Acceptable\r\n"); break;
        case 407: strcat(h, "HTTP/1.1 407 Proxy Authentication Required\r\n"); break;
        case 408: strcat(h, "HTTP/1.1 408 Requuest Timeout\r\n"); break;
        case 409: strcat(h, "HTTP/1.1 409 Conflict\r\\n"); break;
        case 410: strcat(h, "HTTP/1.1 410 Gone\r\n"); break;
        case 411: strcat(h, "HTTP/1.1 411 Lenth Require\r\n"); break;
        case 412: strcat(h, "HTTP/1.1 412 Precondition Failed\r\n"); break;
        case 413: strcat(h, "HTTP/1.1 413 Request Entity Too Large\r\n"); break;
        case 414: strcat(h, "HTTP/1.1 414 Request-url Too Long\r\n"); break;
        case 415: strcat(h, "HTTP/1.1 415 Unsupported Media Type\r\n"); break;
        case 416: strcat(h, "HTTP/1.1 416\r\n"); break;
        case 417: strcat(h, "HTTP/1.1 417 Exceptation Failed\r\n"); break;
        case 500: strcat(h, "HTTP/1.1 500 Internal Server Error\r\n"); break;
        case 501: strcat(h, "HTTP/1.1 501 Not Implemented\r\n"); break;
        case 502: strcat(h, "HTTP/1.1 502 Bad Getaway\r\n"); break;
        case 503: strcat(h, "HTTP/1.1 503 Service Unavailable\r\n"); break;
        case 504: strcat(h, "HTTP/1.1 504 Gateway Timeout\r\n"); break;
        case 505: strcat(h, "HTTP/1.1 505 HTTP Version Not Support\r\n"); break;
        default:
            perror("Unknow state code!");
            exit(1);
    }
    head = h;
    return head;
}

char *get_Date_line(){
    char date[64] = "Date:", *d;
    char *time;
    strcat(date, get_current_time());
    strcat(date, " GMT\r\n");
    d = date;
    return d;
}

char *get_server_line(){
    char *ll = NULL, l[32] = "";
    strcat(l, "Server:Mutu(0.1 Alpha)/Linux\r\n");
    ll = l;
    return ll;
}

char *get_contentE_line(){
    char *cont = NULL, c[32] = "";
    strcat(c, "Context-Encoding:gzip\r\n");
    cont = c;
    return cont;
}

char *get_contentL_line(){
    char *ll = NULL, l[32] = "";
    strcat(l, "Content-Language:en,zh\r\n");
    ll = l;
    return ll;
}

char *get_contentLen_line(int len){
    char le[64] = "Content-Length:", *l = NULL;
    strcat(le , int_to_string(len));
    strcat(le ,"\r\n");
    l = le;
    return l;
}

char *get_contentT_line(enum MIMETYPE type){
    char ty[32] = "Content-Type:", *typ = NULL;
    strcat(ty, type_to_str(type));
    strcat(ty, "\r\n");
    typ = ty;
    return typ;
}

int get_dir_info(char *file_buf, char *dir, struct http_request *req){
    strcpy(file_buf, "<html>\r\n<body>\r\n");
    char port[8] = "";
    char d[64] = "";
    strncpy(d,dir + strlen(ROOT_DIR), strlen(dir) - strlen(ROOT_DIR));
    sprintf(port, "%d", req->port);
    DIR *dirp = NULL;
    struct dirent * direntp;

    if((dirp=opendir(dir))==NULL){
        printf("This dir is inexisted!\n");
        req->state = 404;
        return -1;
    } else {
        while((direntp = readdir(dirp))!=NULL){
            if(strcmp(direntp->d_name,".") == 0 || strcmp(direntp->d_name, "..") ==0){
                continue;
            }
            strcat(file_buf, "<a href=\"");
            strcat(file_buf, d + 1);
            strcat(file_buf, "/");
            strcat(file_buf, direntp->d_name);
            strcat(file_buf, "\">");
            strcat(file_buf, direntp->d_name);
            strcat(file_buf, "</a>\r\n");
        }
        closedir(dirp);
        strcat(file_buf, "</body>\r\n</html>\r\n");
        req->state = 200;
        return 1;
    }

}

int send_normal_msg(struct http_request *req, char *send_buf, int file_byte){
    strcpy(send_buf, get_first_line(req->state));
    strcat(send_buf, get_Date_line());
    strcat(send_buf, get_server_line());
    strcat(send_buf, get_contentE_line());
    strcat(send_buf, get_contentL_line());
    strcat(send_buf, get_contentT_line(req->m_type));
    strcat(send_buf, get_contentLen_line(file_byte + sizeof(send_buf)));
    strcat(send_buf, "\r\n");
    return 0;
}

int send_error_msg(int state, char *send_buf){
    char *error_file = (char *)malloc(128* sizeof(char));
    char *error_file_buf = (char *)malloc(FILE_BUF_SIZE* sizeof(char));
    char st[8] = "";
    int fd= 0;
    strcpy(send_buf, get_first_line(state));
    get_Date_line();
    strcat(send_buf, get_Date_line());
    strcat(send_buf, get_server_line());
    strcat(send_buf, get_contentE_line());
    strcat(send_buf, get_contentL_line());
    strcat(send_buf, get_contentT_line(HTML));
    strcat(send_buf, get_contentLen_line(SEND_BUF_SIZE));
    sprintf(st, "%d", state);
    strcpy(error_file, ROOT_DIR);
    strcat(error_file, "/");
    strcat(error_file, st);
    strcat(error_file, ".html");
    printf("file:%s", error_file);
    if((fd = open(error_file, O_RDONLY)) == -1){
        printf("Error file cannot been open!");
        return -1;
    } else {
        if(read(fd, error_file_buf, FILE_BUF_SIZE) == -1){
            printf("Error file cannot been read!");
            return -1;
        }
    }
    strcat(send_buf, "\r\n");
    strcat(send_buf, error_file_buf);
    strcat(send_buf, "\r\n");
    close(fd);
    return 1;
}
