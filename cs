struct client_desc* head_init(int client_sock, char *client_name){
    struct client_desc *temp = (struct client_desc*)malloc(sizeof(struct client_desc));
    //temp->sock = client_sock;
    temp->name = client_name;
    temp->sock_desc = client_sock;
    temp->next = NULL;
    return temp;
}
struct client_desc* add_new_client(int client_sock, char *client_name) {
    if(head == NULL){
       return head_init(client_sock, client_name);
    }
    struct client_desc* temp = head;
    bool exist = false;
    while(temp->next != NULL){
        if(temp->sock_desc == client_sock){
            exist = true;
            break;
        }
        temp = temp->next;
    }
    if(exist == false){
        temp->name = client_name;
       // temp->sock = client_sock;
        temp->sock_desc = client_sock;
        temp->next = NULL;
    }
    return head;
}






void itreate_network(struct client_desc *re_client, char message[BUF_SIZE]){
    struct client_desc* client = head;
    while(client != NULL){
        if(re_client->sock_desc != client->sock_desc){
            char* msg = strcat(re_client->name," : ");
            if(write(client->sock_desc,strcat(msg,message), BUF_SIZE) < 0){
                continue;
            }
        }
        client = client->next; 
    }
}