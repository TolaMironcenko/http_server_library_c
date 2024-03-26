#ifndef DOCS_H
#define DOCS_H

void docs_page(int conn, HTTPreq *request) {
    if(strcmp(request->path, "/docs/") != 0) {
        parsehtml_http(conn, "web/pages/page404.html");
        return;
    }
    http_response(conn, "web/pages/docs.html", HTML);
}

void docs_page_css(int conn, HTTPreq *request) {
    printf("%s - %s - %s\n", request->method, request->path,  request->proto);
    if(strcmp(request->path, "/docs/docs.css") != 0) {
        parsehtml_http(conn, "web/pages/page404.html");
        return;
    }
    http_response(conn, "web/css/docs.css", CSS);
}

void docs_page_js(int conn, HTTPreq *request) {
    printf("%s - %s - %s\n", request->method, request->path,  request->proto);
    if(strcmp(request->path, "/docs/docs.js") != 0) {
        parsehtml_http(conn, "web/pages/page404.html");
        return;
    }
    http_response(conn, "web/js/docs.js", JS);
}

#endif // DOCS_H