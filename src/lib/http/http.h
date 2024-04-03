//
// Created by Анатолий Миронченко on 24.10.2022.
//

#ifndef HTTP_SERVER_C_HTTP_H
#define HTTP_SERVER_C_HTTP_H

#include <stdint.h>
#include <stdlib.h>

typedef struct HTTPreq {
    char method[16];
    char path[2048];
    char proto[16];
    uint8_t state;
    size_t index;
} HTTPreq;

typedef struct {
    int conn;
    char filename[1024];
} param;

typedef struct HTTP HTTP;

extern HTTP *new_http(char *address);
extern void free_http(HTTP *http);

extern void handle_http(HTTP *http, char *path, void(*)(int, HTTPreq*));
extern int8_t listen_http(HTTP *http);

extern void parsehtml_http(int conn, char *filename);

extern void json_response(int conn, char *json);

extern void http_response(int conn, char *filename, char *filetype_http);

extern void send_video(void *arg);

extern void http_response404(int conn, char *filename, char *filetype_http);

#endif //HTTP_SERVER_C_HTTP_H
