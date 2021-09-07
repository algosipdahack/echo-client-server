#include "header.h"
int main(int argc, char * argv[]){
    int serv_sock;
    int clnt_sock;
    pthread_t t_id[100];
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    char buf[1024];
    if(argc<2){
        printf("syntax: echo-server <port> [-e[-b]]\n");
        printf("sample: echo-server 1234 -e -b\n");
        exit(1);
    }

    pthread_mutex_init(&mutx, NULL);
    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock==-1)
        error_handling("socket() error");

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
        error_handling("bind errror");
    if(listen(serv_sock,5)==-1)
        error_handling("listen error");

    if(!strcmp(argv[2],"-e"))flag = 1;
    else if(!strcmp(argv[2],"-b"))flag = 2;
    while(1){
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
        clisock_list[num_user] = clnt_sock;
        if(clnt_sock==-1)
            error_handling("accept error");
        pthread_create(&t_id[num_user], NULL, handle_clnt, (void*)&clnt_sock);
        pthread_detach(t_id[num_user++]);
    }
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void* handle_clnt(void* arg){
    int clnt_sock = *((int*)arg);
    int str_len = 0;
    char msg[1024];

    while ((str_len = read(clnt_sock, msg, sizeof(msg))) != 0) {
        int i;
        if(flag==1)
            write(clnt_sock,msg,sizeof(msg));
        else if(flag==2)
            for(int i =0; i<num_user; i++)
                write(clisock_list[i],msg,sizeof(msg));

        printf("message : %s\n",msg);
    }
    return NULL;
}

void error_handling(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
