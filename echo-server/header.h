#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string>

int clisock_list[100];
int num_user = 0;
int flag = 0;
pthread_mutex_t mutx;
void* handle_clnt(void* arg);
void send_msg(char* msg, int len,int choose);
void error_handling(std::string message);
int echo = 0;
int broad = 0;
