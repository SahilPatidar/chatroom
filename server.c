#include<stdio.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<signal.h>

#define BUF_SIZE 3000
void* connection_handler(void*);

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("usage %s <port>",argv[0]);
        return EXIT_FAILURE;
    }
    int sock_len;
    int port = atoi(argv[1]);
    int socket_desc,new_socket,*new_sock;
    struct sockaddr_in server,client;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_desc == -1){
        printf("socket_desc failed");
        return EXIT_FAILURE;
    }
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    signal(SIGPIPE, SIG_IGN);

    if(bind(socket_desc, (struct sockaddr*)&server,sizeof(server)) < 0){
        printf("binding failed");
        return EXIT_FAILURE;
    }

    if(listen(socket_desc, 3) < 0){
        printf("listening failed");
        return 1;
    }
    sock_len = sizeof(struct sockaddr_in);

    while((new_socket = accept(socket_desc, (struct sockaddr*)&client, (socklen_t*)&sock_len)) > 0){
        pthread_t pid;
        new_sock = malloc(1);
        new_sock = new_socket;
        if(write(new_socket, "enter your name:",strle("enter your name:")) < 0){
            printf("> write falied...\n");
        }
        pthread_t pid;
        if(pthread_create(&pid, NULL, connection_handler, (void*)new_sock) < 0)
         {
			perror("could not create thread");
			return 1;
		}
    }

     if(new_socket < 0){
        perror("accept failed\n");
    }
    
return 0;

}

void* connection_handler(void* sock_desc){
    char buf[BUF_SIZE];

    return 0;
}