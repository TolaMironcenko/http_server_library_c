//
// Created by Анатолий Миронченко on 24.10.2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "../hashtab/hashtab.h"
#include "../net/net.h"
#include "http.h"

#define METHOD_SIZE 16
#define PATH_SIZE   2048
#define PROTO_SIZE  16

#define BUFFERSIZE 8192

typedef struct HTTP {
    char *host;
    int32_t len;
    int32_t cap;
    void(**funcs)(int, HTTPreq*);
    HashTab *tab;
} HTTP;

static HTTPreq _new_request(void);
static void _parse_request(HTTPreq *request, char *buffer, size_t size);
static void _null_request(HTTPreq *request);
static int8_t _switch_http(HTTP *http, int conn, HTTPreq *request);
static void _page404_http(int conn);

extern HTTP *new_http(char *address) {
    HTTP *http = (HTTP*)malloc(sizeof(HTTP));
    http->cap = 1000;
    http->len = 0;
    http->host = (char*)malloc(sizeof(char)*strlen(address)+1);
    strcpy(http->host, address);
    http->tab = new_hashtab(http->cap, STRING_TYPE, DECIMAL_TYPE);
    http->funcs = (void(**)(int, HTTPreq*))malloc(http->cap * (sizeof (void(*)(int, HTTPreq*))));
    return http;
}

extern void free_http(HTTP *http) {
    free_hashtab(http->tab);
    free(http->host);
    free(http->funcs);
    free(http);
}

extern void handle_http(HTTP *http, char *path, void(*handle)(int, HTTPreq*)) {
    set_hashtab(http->tab, string(path), decimal(http->len));
    http->funcs[http->len] = handle;
    http->len += 1;
    if (http->len == http->cap) {
        http->cap <<= 1;
        http->funcs = (void(**)(int, HTTPreq*))realloc(http->funcs,
                                                       http->cap * (sizeof (void(*)(int, HTTPreq*))));
    }
//    println_hashtab_format(http->tab);
}

extern int8_t listen_http(HTTP *http) {
    int listener = listen_net(http->host);
    if (listener < 0) {
        return 1;
    }
    while(1) {
        int conn = accept_net(listener);
        if (conn < 0) {
            continue;
        }
        HTTPreq req = _new_request();
        while(1) {
            char buffer[BUFFERSIZE] = {0};
            int n = recv_net(conn, buffer, BUFFERSIZE);
            if (n < 0) {
                break;
            }
            _parse_request(&req, buffer, n);
            if (n != BUFFERSIZE) {
                break;
            }
        }
        _switch_http(http, conn, &req);
        close_net(conn);
    }
    close_net(listener);
    return 0;
}

extern void parsehtml_http(int conn, char *filename) {
    char buffer[BUFFERSIZE] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
    size_t readsize = strlen(buffer);
    send_net(conn, buffer, readsize);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }
    while((readsize = fread(buffer, sizeof(char), BUFFERSIZE, file)) != 0) {
        send_net(conn, buffer, readsize);
    }
    fclose(file);
}

static HTTPreq _new_request(void) {
    return (HTTPreq){
            .method = {0},
            .path = {0},
            .proto = {0},
            .state = 0,
            .index = 0,
    };
}

static void _parse_request(HTTPreq *request, char *buffer, size_t size) {
    // printf("%s\n", buffer);
    for (size_t i = 0; i < size; ++i) {
        switch(request->state) {
            case 0:
                if (buffer[i] == ' ' || request->index == METHOD_SIZE-1) {
                    request->method[request->index] = '\0';
                    _null_request(request);
                    continue;
                }
                request->method[request->index] = buffer[i];
                break;
            case 1:
                if (buffer[i] == ' ' || request->index == PATH_SIZE-1) {
                    request->path[request->index] = '\0';
                    _null_request(request);
                    continue;
                }
                request->path[request->index] = buffer[i];
                break;
            case 2:
                if (buffer[i] == '\n' || request->index == PROTO_SIZE-1) {
                    request->proto[request->index] = '\0';
                    _null_request(request);
                    continue;
                }
                request->proto[request->index] = buffer[i];
                break;
            default: return;
        }
        request->index += 1;
    }
}

static void _null_request(HTTPreq *request) {
    request->state += 1;
    request->index = 0;
}

static int8_t _switch_http(HTTP *http, int conn, HTTPreq *request) {
    if (!in_hashtab(http->tab, string(request->path))) {
        char buffer[PATH_SIZE];
        memcpy(buffer, request->path, PATH_SIZE);
        int32_t index = strlen(request->path);
        if (index == 0) {
            _page404_http(conn);
            return 1;
        }
        index -= 1;
        for (; index > 0 && buffer[index] != '/'; --index) {
            buffer[index] = '\0';
        }
        if(!in_hashtab(http->tab, string(buffer))) {
            _page404_http(conn);
            return 2;
        }
        index = get_hashtab(http->tab, string(buffer)).decimal;
        http->funcs[index](conn, request);
        return 0;
    }
    int32_t index = get_hashtab(http->tab, string(request->path)).decimal;
    http->funcs[index](conn, request);
    return 0;
}

extern void http_response404(int conn, char *filename, char *filetype_http) {
    char buffer[BUFFERSIZE] = "HTTP/1.1 404 not found\r\n"
                          "Version: HTTP/1.1\r\n"
                          "Content-type: ";
    strcat(buffer, filetype_http);
    strcat(buffer, "\r\n\r\n");
    size_t readsize = strlen(buffer);
    send_net(conn, buffer, readsize);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }
    while((readsize = fread(buffer, sizeof(char), BUFFERSIZE, file)) != 0) {
        send_net(conn, buffer, readsize);
    }
    fclose(file);
}

static void _page404_http(int conn) {
    http_response404(conn, "web/pages/page404.html", "text/html");
}

extern void json_response(int conn, char *json) {
    char buffer[BUFFERSIZE] = "HTTP/1.1 200 OK\r\n"
                          "Version: HTTP/1.1\r\n"
                          "Content-type: application/json\r\n\r\n";
    strcat(buffer, json);
    size_t readsize = strlen(buffer);
    send_net(conn, buffer, readsize);
}

extern void http_response(int conn, char *filename, char *filetype_http) {
    char buffer[BUFFERSIZE] = "HTTP/1.1 200 OK\r\n"
                          "Version: HTTP/1.1\r\n"
                          "Content-type: ";
    strcat(buffer, filetype_http);
    strcat(buffer, "\r\n\r\n");
    size_t readsize = strlen(buffer);
    send_net(conn, buffer, readsize);
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return;
    }
    while((readsize = fread(buffer, sizeof(char), BUFFERSIZE, file)) > 0) {
        send_net(conn, buffer, readsize);
    }
    fclose(file);
}

extern void send_video(void *arg) {
    param * p = (param *)arg;
    int conn = p->conn;
    char filename[1024];
    memset(&filename, '\0', 1024);
    strcpy(filename, p->filename);

    char buffer[BUFFERSIZE];
    int chunk_size = 4096;
    char chunk_buffer[chunk_size];
    int start_byte = 0;
    int end_byte = 0;
    memset(&buffer, '\0', BUFFERSIZE);
    memset(&chunk_buffer, '\0', chunk_size);
    // sprintf(buffer, "HTTP/1.1 200 OK\r\n"
    //                 "Version: HTTP/1.1\r\n"
    //                 "Content-Range: bytes %d-%d\r\n"
    //                 "Accept-Ranges: bytes\r\n"
    //                 "Content-Length: %d\r\n"
    //                 "Content-type: video/mp4\r\n"
    //                 "\r\n", start_byte, end_byte, chunk_size);

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return;
    }

    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    int bytes_read;
    while((bytes_read = fread(chunk_buffer, 1, chunk_size, file)) > 0) {
        // printf("%s\n\n", chunk_buffer);
        start_byte = end_byte;
        end_byte += chunk_size;

        memset(&buffer, '\0', BUFFERSIZE);
        sprintf(buffer, "HTTP/1.1 200 OK\r\n"
                    "Version: HTTP/1.1\r\n"
                    "Content-Range: bytes %d-%d/%d\r\n"
                    "Accept-Ranges: bytes\r\n"
                    "Content-Length: %d\r\n"
                    "Content-type: video/mp4\r\n"
                    "\r\n"
                    "%s", start_byte, end_byte, file_size, bytes_read, chunk_buffer);
        
        memset(chunk_buffer, '\0', chunk_size);
        printf("%s\n\n", buffer);
        send_net(conn, buffer, BUFFERSIZE);
        // sleep(2);
    }
    fclose(file);
    close_net(conn);
    // long file_size = 
}