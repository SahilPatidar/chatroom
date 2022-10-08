#include<stdio.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<string.h>

#define BUF_SIZE 3000

void* recv_thread(void* );
void* send_thread(void* );
void catch(int sig){
    exit(EXIT_SUCCESS);
}
void str_end(char *arr, int length){
   int i;
  for (i = 0; i < length; i++) { // trim \n
    if (arr[i] == '\n') {
        arr[i] = '\0';
        break;
    }
  }
}
int main(int argc, char *argv[]){
    if(argc < 2){
        printf("usage %s <port>",argv[0]);
        return EXIT_FAILURE;
    }
    int port = atoi(argv[1]);

    int socket_desc,*new_sock;
    struct sockaddr_in client;
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    client.sin_addr.s_addr = INADDR_ANY;
    client.sin_family = AF_INET;
    client.sin_port = htons(port);
    signal(SIGINT, catch);

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
    }
    char message[BUF_SIZE];
    new_sock = malloc(1);
    *new_sock = socket_desc;
    pthread_t read_thread;
    pthread_t write_thread;
    pthread_create(&read_thread, NULL, recv_thread, (void*)new_sock);
    pthread_create(&write_thread, NULL, send_thread, (void*)new_sock);

    pthread_join(read_thread, NULL);
    pthread_join(write_thread, NULL);

    close(socket_desc);

    return 0;
}



void* recv_thread(void* sock_desc){
    while (1)
    {   printf("> ");
        char recv_msg[BUF_SIZE];
        int sock = *(int*)sock_desc;
        if(recv(sock, recv_msg, BUF_SIZE,0) < 0){
            printf("recv falied...");
        }
        printf("%s\n",recv_msg);
    }
    return 0;
}

void* send_thread(void* sock_desc){
    char message[BUF_SIZE] = {};

    while (1)
    {   
        printf("> ");
        fflush(stdin);
        fgets(message,BUF_SIZE,stdin);
        str_end(message,BUF_SIZE);
        int sock = *(int*)sock_desc;
        if(strcmp(message, ".exit")){
            send(sock, message, strlen(message)+1,0);
            goto ret;
        }
        if(send(sock, message, strlen(message)+1,0) < 0){
            printf("send falied...");
        }
        bzero(message,BUF_SIZE);
    }
ret:
    return 0;
}

