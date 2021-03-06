#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "../src/SSLHandler.h"

void error_handling(char *message);

int main(int argc, char *argv[])
{


    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[]="Hello World!";


    serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(10081);

    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
        error_handling("bind() error");

    if(listen(serv_sock, 5)==-1)
        error_handling("listen() error");

    clnt_addr_size=sizeof(clnt_addr);
    clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
    auto ca = CA();
    ca.Loads("5327_131669_44085 44511 10430 21968 30565 20006 44511 20006 43434 43434 47454");
    auto ssl = SSLHandler(clnt_sock, "5327_131669", "33455_131669", "10651_56153", ca);
    auto s = ssl.DoShakeHandsServer();
    if(clnt_sock==-1)
        error_handling("accept() error");

    std::string msg;
    ssl.S_Read(msg);

    std::cout<<msg<<std::endl;

    ssl.S_Write(message);


    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
