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
#define NAME_SIZE 32
#define TOTAL_BUF NAME_SIZE+BUF_SIZE

struct client_desc{
    struct sockaddr_in sock_in;
    char name[NAME_SIZE];
    int sock_desc;
    struct client_desc *next;
};

struct client_desc* head;
struct client_desc* tall;
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;

void str_end(char *arr, int length){
   int i;
  for (i = 0; i < length; i++) { // trim \n
    if (arr[i] == '\n') {
        arr[i] = '\0';
        break;
    }
  }
}
void add_client_desc(struct client_desc *client_t) {
    pthread_mutex_lock(&client_mutex);
    if(head == NULL){
        head = client_t;
        tall = head;
    }else{
        tall = tall->next;
        tall = client_t;
    }
    pthread_mutex_unlock(&client_mutex);
}

void remove_client_desc(int socket) {
    pthread_mutex_lock(&client_mutex);
    
    if(head->next==NULL){
        struct client_desc *todel = head;
        head = head->next;
        free(todel);
        todel = NULL;
    }
    struct client_desc *temp = head;
    while(temp->next->sock_desc!=socket){
        temp = temp->next;
    }
    struct client_desc* todel = temp->next;
    temp->next = temp->next->next;
    free(todel);
    todel = NULL;
    pthread_mutex_unlock(&client_mutex);
}

void iterate_client(int socket, char message[TOTAL_BUF]){
    pthread_mutex_lock(&client_mutex);
    struct client_desc* temp = head;
    while(temp!=NULL){
        if(temp->sock_desc != socket){
            write(temp->sock_desc, message, TOTAL_BUF-1);
        }
        temp = temp->next;
    }
    pthread_mutex_unlock(&client_mutex);
}

void* connection_handler(void* client_t){
    
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("usage %s <port>",argv[0]);
        return EXIT_FAILURE;
    }
    int sock_len;
    int port = atoi(argv[1]);
    int socket_desc,new_socket;
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
        printf("binding failed\n");
        return EXIT_FAILURE;
    }

    if(listen(socket_desc, 3) < 0){
        printf("listening failed");
        return 1;
    }
    sock_len = sizeof(struct sockaddr_in);
    int read_ = -1;
    printf("accepting ......\n");
    while((new_socket = accept(socket_desc, (struct sockaddr*)&client, (socklen_t*)&sock_len)) > 0){
        write(new_socket, "  ------======Welcome to chat server=====-----\n", strlen("------======Welcome to chat server=====-----"));
        struct client_desc* client_t = (struct client_desc*)malloc(sizeof(struct client_desc));
        client_t->sock_in = client;
        client_t->sock_desc = new_socket;
        pthread_t pid;
        if(pthread_create(&pid, NULL, &connection_handler,(void*)client_t) < 0)
        {
			perror("could not create thread");
			return 1;
		}
        sleep(1);
    }

    if(new_socket < 0){
        perror("accept failed\n");
    }
    
return 0;

}


