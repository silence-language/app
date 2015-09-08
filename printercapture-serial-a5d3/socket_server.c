#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#define SERVERNAME "/home/server_socket"
void *server_init()
{
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_un server_address;
    struct sockaddr_un client_address;
    unlink(SERVERNAME);
    server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    
    if (server_sockfd<0)
        {
            printf("[%s]:%d", __FILE__, __LINE__);
            exit(1);
        }
    
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, SERVERNAME);
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
    printf("server for socket is runing.....!\n");
    listen(server_sockfd, 5);
    while(1)
        {
            char ch[12];
            printf("server_waitin.......\n");
            client_len = sizeof(client_address);
            client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);

            read(client_sockfd, &ch[0], 12);
            printf("received : %c", ch[0]);
            ch[0]++;
            write(client_sockfd, &ch, 1);
            close(client_sockfd);
        }
}
/* int  main() */
/* { */
/*     server_init(); */
/* } */
