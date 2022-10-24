//
// Created by Анатолий Миронченко on 24.10.2022.
//

#include <stdio.h>
#include <stdlib.h>
#include "lib/http/http.h"
#include <string.h>

void index_page(int conn, HTTPreq *request) {
    printf("%s - %s - %s\n", request->method, request->path,  request->proto);
    if(strcmp(request->path, "/") != 0) {
        http_response(conn, "web/pages/page404.html", "text/html");
        return;
    }
    http_response(conn, "web/pages/index.html", "text/html");
}

void index_page_js(int conn, HTTPreq *request) {
    printf("%s - %s - %s\n", request->method, request->path,  request->proto);
    if(strcmp(request->path, "/index.js") != 0) {
        http_response(conn, "web/pages/page404.html", "text/html");
        return;
    }
    http_response(conn, "web/js/index.js", "text/javascript; charset=utf-8");
}

void index_page_css(int conn, HTTPreq *request) {
    printf("%s - %s - %s\n", request->method, request->path,  request->proto);
    if(strcmp(request->path, "/index.css") != 0) {
        http_response(conn, "web/pages/page404.html", "text/html");
        return;
    }
    http_response(conn, "web/css/index.css", "text/css");
}

void json_response_http(int conn, HTTPreq *request) {
    printf("%s - %s - %s\n", request->method, request->path,  request->proto);
    if(strcmp(request->path, "/message/") != 0) {
        http_response(conn, "web/pages/page404.html", "text/html");
        return;
    }
    json_response(conn, "{\"message\" : \"hello world\", \"username\" : \"tola\"}");
}

void about_page(int conn, HTTPreq *request) {
    if(strcmp(request->path, "/about/") != 0) {
        parsehtml_http(conn, "web/pages/page404.html");
        return;
    }
    http_response(conn, "web/pages/about.html", "text/html");
}

void docs_page(int conn, HTTPreq *request) {
    if(strcmp(request->path, "/docs/") != 0) {
        parsehtml_http(conn, "web/pages/page404.html");
        return;
    }
    http_response(conn, "web/pages/docs.html", "text/html");
}

void docs_page_css(int conn, HTTPreq *request) {
    printf("%s - %s - %s\n", request->method, request->path,  request->proto);
    if(strcmp(request->path, "/docs/docs.css") != 0) {
        parsehtml_http(conn, "web/pages/page404.html");
        return;
    }
    http_response(conn, "web/css/docs.css", "text/css");
}

void docs_page_js(int conn, HTTPreq *request) {
    printf("%s - %s - %s\n", request->method, request->path,  request->proto);
    if(strcmp(request->path, "/docs/docs.js") != 0) {
        parsehtml_http(conn, "web/pages/page404.html");
        return;
    }
    http_response(conn, "web/js/docs.js", "text/javascript; charset=utf-8");
}

int main(void) {
    system("clear");

    HTTP *serve = new_http("127.0.0.1:54321");

    handle_http(serve, "/", index_page);
    handle_http(serve, "/about/", about_page);
    handle_http(serve, "/message/", json_response_http);
    handle_http(serve, "/index.js", index_page_js);
    handle_http(serve, "/index.css", index_page_css);
    handle_http(serve, "/docs/", docs_page);
    handle_http(serve, "/docs/docs.css", docs_page_css);
    handle_http(serve, "/docs/docs.js", docs_page_js);

    listen_http(serve);
    free_http(serve);
    return 0;
}