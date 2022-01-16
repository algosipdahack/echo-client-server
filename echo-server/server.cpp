#include "header.h"
int makesocket(char* argv[])
{
    int serv_sock;
    struct sockaddr_in serv_addr;

    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock==-1) {
        error_handling("socket() error");
    }

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1) {
        error_handling("bind errror");
    }
    if(listen(serv_sock,5)==-1) {
        error_handling("listen error");
    }
    return serv_sock;
}

void makeConnect(int serv_sock)
{
    socklen_t clnt_addr_size;
    struct sockaddr_in clnt_addr;
    pthread_t t_id[100];
    int clnt_sock;

    while(1)
    {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
        clisock_list[num_user] = clnt_sock;
        if(clnt_sock==-1) {
            error_handling("accept error");
        }
        pthread_create(&t_id[num_user], NULL, handle_clnt, (void*)&clnt_sock);
        pthread_detach(t_id[num_user++]);
    }
    close(clnt_sock);
    close(serv_sock);
}

int parsing(int argc, char* argv[])
{
    if(argc < 2) {
        return 0;
    }
    for(int i = 2; i<argc; i++)
    {
        if(!strcmp(argv[i],"-e"))echo = 1;
        if(!strcmp(argv[i],"-b"))broad = 1;
    }
    return 1;
}

void* handle_clnt(void* arg)
{
    int clnt_sock = *((int*)arg);
    char msg[1024];

    while ((read(clnt_sock, msg, sizeof(msg))) != 0)
    {
        if(echo) {
            write(clnt_sock,msg,sizeof(msg));
        }
        if(broad) {
            for(int i =0; i<num_user; i++) {
                write(clisock_list[i],msg,sizeof(msg));
            }
        }
        printf("message : %s\n",msg);
    }
    return NULL;
}

void error_handling(std::string message)
{
    fputs(message.c_str(),stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc, char * argv[]) {
    if(!parsing(argc,argv))
    {
        printf("syntax: echo-server <port> [-e[-b]]\n");
        printf("sample: echo-server 1234 -e -b\n");
        exit(1);
    }
    int serv_sock = makesocket(argv);
    makeConnect(serv_sock);
    return 0;
}
