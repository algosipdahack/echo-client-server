#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string>
using namespace std;
void error_handling(std::string message);
void* recv_msg(void* arg);
void* send_msg(void* arg);
int makesocket(char* argv[]);
