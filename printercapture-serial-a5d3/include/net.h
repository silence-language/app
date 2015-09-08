#ifndef __NET_H
#define __NET_H
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <netinet/in.h>
//#include <arpa/inet>
#include <unistd.h>
#include <stdlib.h>
#include "general.h"
#define PRINT_ERR(X) printf("%s", X)
#define SERVER_PORT 6666
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512
int client_init();

static inline int SendToServer(int socket, char *buffer, int length)
{
   return send(socket, buffer, length, 0);
}
#endif
