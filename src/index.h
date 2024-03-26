#ifndef INDEX_H
#define INDEX_H

void index_page(int conn, HTTPreq *request) {
    printf("%s - %s - %s\n", request->method, request->path,  request->proto);
    if(strcmp(request->path, "/") != 0) {
        http_response(conn, "web/pages/page404.html", HTML);
        return;
    }
    http_response(conn, "web/pages/index.html", HTML);
}

void index_page_js(int conn, HTTPreq *request) {
    printf("%s - %s - %s\n", request->method, request->path,  request->proto);
    if(strcmp(request->path, "/index.js") != 0) {
        http_response(conn, "web/pages/page404.html", HTML);
        return;
    }
    http_response(conn, "web/js/index.js", JS);
}

void index_page_css(int conn, HTTPreq *request) {
    printf("%s - %s - %s\n", request->method, request->path,  request->proto);
    if(strcmp(request->path, "/index.css") != 0) {
        http_response(conn, "web/pages/page404.html", HTML);
        return;
    }
    http_response(conn, "web/css/index.css", CSS);
}

void favicon(int conn, HTTPreq *request) {
    printf("%s - %s - %s\n", request->method, request->path,  request->proto);
    if (strcmp(request->path, "/favicon.ico") != 0) {
        http_response(conn, "web/pages/page404.html", HTML);
        return;
    }
    http_response(conn, "web/images/icon.png", IMAGE_PNG);
}

#endif // INDEX_H