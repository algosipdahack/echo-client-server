#include "header.h"
int makesocket(char* argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    sock = socket(PF_INET, SOCK_STREAM,0);

    if(sock==-1){
        error_handling("socket error");
    }

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) {
        error_handling("connect error");
    }
    return sock;
}

void* send_msg(void* arg)
{
    int sock=*((int*)arg);
    char buf[1024];

    while(1)
    {
        memset(buf, 0, sizeof(buf));
        scanf("%s",buf);
        write(sock, buf, sizeof(buf));
    }
    return NULL;
}

void* recv_msg(void* arg)
{
    int sock=*((int*)arg);
    char name_msg[1024];
    int str_len;

    while(1){
        str_len=read(sock, name_msg, sizeof(name_msg));
        if (str_len==-1) {
            error_handling("read error!");
        }
        printf("message : %s\n",name_msg);
    }
    return NULL;
}

void error_handling(std::string message)
{
    fputs(message.c_str(),stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc, char* argv[])
{
    pthread_t snd_thread, rcv_thread;

    if(argc!=3){
        printf("syntax: echo-client <ip> <port>\n");
        printf("sample: echo-client 192.168.10.2 1234\n");
        exit(1);
    }

    int sock = makesocket(argv);
    pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
    pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
    pthread_detach(snd_thread);
    pthread_join(rcv_thread, NULL);
    close(sock);
    return 0;
}
