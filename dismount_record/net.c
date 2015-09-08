#include "include/net.h"

/* int client_init() */
/* { */
/*     struct sockaddr_in client_addr; */
/*     bzero(&client_addr, sizeof(client_addr)); */
/*     client_addr.sin_family = AF_INET; */
/*     client_addr.sin_addr.s_addr = htons(INADDR_ANY); */
/*     client_addr.sin_port = htons(0); */
/*     int client_socket = socket(AF_INET, SOCK_STREAM, 0); */
/*     if(client_socket < 0) */
/*         { */
/*             PRINTERR("Creat Socket Failed! \n"); */
/*             exit(1); */
/*         } */
/*     if(bind(client_socket, (struct sockaddr*)&client_addr, sizeof(client_addr))) */
/*         { */
/*             PRINTERR("Client bind port failed!\n"); */
/*             exit(1); */
/*         } */
/*     struct sockaddr_in server_addr; */
/*     bzero(&server_addr, sizeof(server_addr)); */
/*     server_addr.sin_family = AF_INET; */
/*     server_addr.sin_addr.s_addr = inet_addr("192.168.9.95"); */
/*     server_addr.sin_port = htons(SERVER_PORT); */
/*     socklen_t server_addr_length = sizeof(server_addr); */
/*     if (connect(client_socket, (struct sockaddr*)&server_addr, server_addr_length)<0) */
/*         { */
/*             PRINTERR("Can not Connect to 192.168.9.189"); */
/*             exit(1); */
/*         } */
/*     return client_socket;     */
/*     // close(client_socket); */
        
/* } */

#if 0
int server_init()
{
    int server_socket;
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);
    
    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
        {
            PRINT_ERR("Creat Socket Failed!");
            return -1;
        }
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)))
        {
            //PRINT_ERR("Server Bind Port : %d Failed!", SERVER_PORT);
            return -1;
        }
    if (listen(server_socket, LENGTH_OF_LISTEN_QUEUE))
        {
            PRINT_ERR("Server Listen Failed!");
            return -1;
        }

    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    for(;;)
        {
            int new_server_socket = accept(server_socket, (struct sockaddr*)&client_addr, &length);
            if (new_server_socket<0)
                {
                    PRINT_ERR("Server accept failed!");
                    
                }
            char buffer[BUFFER_SIZE];
            bzero(buffer, BUFFER_SIZE);

            while(1)
                {
          
                    length = recv(new_server_socket, buffer, BUFFER_SIZE, 0);
                    if (length>0)
                        {
                            printf("The received data is: \n%s\n",buffer);
                        }
                    else if (0==length)
                        {
                            (0==length) ? printf("Client socket closed!\n"):PRINT_ERR("Server Recieve data failed!\n"); 
                            close(new_server_socket);
                            break;
                        }                        
                }
        }
}
#endif
#if 0
int main()
{
    struct hostent *hp;
    hp = gethostent();
    printf("%s\n", hp->h_name);
        //printf("%s\n", hp->)
    server_init();
}
#endif
