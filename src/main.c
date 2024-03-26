//
// Created by Анатолий Миронченко on 24.10.2022.
//

#include <stdio.h>
#include <stdlib.h>
#include "lib/http/http.h"
#include "lib/http/mime_types.h"
#include <string.h>
#include "index.h"
#include "docs.h"

void json_response_http(int conn, HTTPreq *request) {
    printf("%s - %s - %s\n", request->method, request->path,  request->proto);
    if(strcmp(request->path, "/message/") != 0) {
        http_response(conn, "web/pages/page404.html", HTML);
        return;
    }
    json_response(conn, "{\"message\" : \"hello govno\", \"username\" : \"nuhai bebru\"}");
}

void about_page(int conn, HTTPreq *request) {
    if(strcmp(request->path, "/about/") != 0) {
        parsehtml_http(conn, "web/pages/page404.html");
        return;
    }
    http_response(conn, "web/pages/about.html", HTML);
}

void video(int conn, HTTPreq *request) {
    printf("%s - %s - %s\n", request->method, request->path,  request->proto);
    if(strcmp(request->path, "/video.mp4") != 0) {
        parsehtml_http(conn, "web/pages/page404.html");
        return;
    }
    http_response(conn, "video.mp4", "video/mp4");
}

void image(int conn, HTTPreq *request) {
    printf("%s - %s - %s\n", request->method, request->path,  request->proto);
    if(strcmp(request->path, "/image.png") != 0) {
        parsehtml_http(conn, "web/pages/page404.html");
        return;
    }
    http_response(conn, "image.png", IMAGE_PNG);
}

int main(void) {
    system("clear");

    HTTP *serve = new_http("127.0.0.1:43243");

    handle_http(serve, "/", index_page);
    handle_http(serve, "/favicon.ico", favicon);
    handle_http(serve, "/index.js", index_page_js);
    handle_http(serve, "/index.css", index_page_css);
    handle_http(serve, "/about/", about_page);
    handle_http(serve, "/message/", json_response_http);
    handle_http(serve, "/docs/", docs_page);
    handle_http(serve, "/docs/docs.css", docs_page_css);
    handle_http(serve, "/docs/docs.js", docs_page_js);
    handle_http(serve, "/video.mp4", video);
    handle_http(serve, "/image.png", image);


    listen_http(serve);
    free_http(serve);
    return 0;
}