#include<stdio.h>
#include<stdbool.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<signal.h>

#define BUF_SIZE 3000

struct client_desc{
    struct sockaddr_in sock;
    char* name;
    int sock_desc;
    struct client_desc *next;
};

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
        *new_sock = new_socket;
     
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
struct client_desc* head_init(int sock, struct sockaddr_in client_sock, char *client_name, struct client_desc* head){
    struct client_desc *temp = (struct client_desc*)malloc(sizeof(struct client_desc));
    temp->sock = client_sock;
    temp->name = client_name;
    temp->sock_desc = sock;
    temp->next = NULL;
    return temp;
}
struct client_desc* add_new_client(int sock, struct sockaddr_in client_sock, char *client_name, client_desc* head) {
    if(head == NULL){
       return head_init(sock, client_sock, client_name, head);
    }
    struct client_desc* temp = head;
    bool exist = false;
    while(temp->next != NULL){
        if(temp->sock.sin_addr.s_addr == client_sock.sin_addr.s_addr){
            exist = true;
            break;
        }
        temp = temp->next;
    }
    if(exist == false){
        temp->name = client_name;
        temp->sock = client_sock;
        temp->sock_desc = sock;
        temp->next = NULL;
    }
    return head;
}

void itreate_network(struct sockaddr_in re_client,struct client_desc *head, char message[BUF_SIZE]){
    struct client_desc* client = head;
    while(client != NULL){
        if(re_client.sin_addr.s_addr != client->sock.sin_addr.s_addr){
            if(write(client->sock_desc,message, BUF_SIZE) < 0){
                printf("failed..");
            }
        }
    }
}

void* connection_handler(void* sock_desc){
    char buf[BUF_SIZE];
    if(write(sock_desc, "enter your name:",strle("enter your name:")) < 0){
        printf("> write falied...\n");
    }
    
    return 0;
}


