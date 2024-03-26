//
// Created by Анатолий Миронченко on 24.10.2022.
//

#ifndef HTTP_SERVER_C_NET_H
#define HTTP_SERVER_C_NET_H

#include <stddef.h>

extern int listen_net(char *address);
extern  int accept_net(int listener);

extern int connect_net(char *address);
extern int close_net(int conn);

extern int send_net(int conn, char *buffer, size_t size);
extern int recv_net(int conn, char *buffer, size_t size);

#endif //HTTP_SERVER_C_NET_H
