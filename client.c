#include<stdio.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<arpa/inet.h>

#define BUF_SIZE 3000
int main(int argc, char *argv[]){
    if(argc < 2){
        printf("usage %s <port>",argv[0]);
        return EXIT_FAILURE;
    }
    int port = atoi(argv[1]);

    int socket_desc;
    struct sockaddr_in client;
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    client.sin_addr = INADDR_ANY;
    client.sin_family = AF_INET;
    client.sin_port = htons(port);
    printf("connecting......");
retry:
    if(connect(socket_desc, (struct sockaddr*)&client, sizeof(client)) < 0) {
        printf("connection failed......\n");
        printf("retry (y) otherwise (n):");
        char in;
        scanf("%c",&in);
        switch(in){
            case ('y'):
               goto retry;
            case ('n'):
                printf("connection terminate..\n");
                return 1;
            default:
                printf("unknown...\n");
                return 1;
    }
    char message[BUF_SIZE];

    while(recv(socket_desc, message, BUF_SIZE, 0) < 0){
        printf("-> %s", )
    }


}