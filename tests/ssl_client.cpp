#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<errno.h>

#include "../src/SSLHandler.h"

int main(int argc,char *argv[])
{
    int sockfd;
    char sendbuffer[200];
    char recvbuffer[200];
    //  char buffer[1024];
    struct sockaddr_in server_addr;
    struct hostent *host;
    int portnumber,nbytes;
    host = gethostbyname("0.0.0.0");
    if((portnumber=10081)<0)
    {
        fprintf(stderr,"Usage:%s hostname portnumber\a\n",argv[0]);
        exit(1);
    }
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        fprintf(stderr,"Socket Error:%s\a\n",strerror(errno));
        exit(1);
    }
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(portnumber);
    server_addr.sin_addr=*((struct in_addr *)host->h_addr);
    if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
    {
        fprintf(stderr,"Connect error:%s\n",strerror(errno));
        exit(1);
    }
    auto ca = CA();
    ca.Loads("45257_227891_31939 44085 10430 44085 21968 30565 10430 10430 21968 34470 47454 20006 ");
    auto ssl = SSLHandler(sockfd, "45257_227891", "82793_227891", "10651_56153", ca);
    auto s = ssl.DoShakeHandsClient();

    std::string read_buf;
    printf("\n");
    ssl.S_Write("HELLO");
    ssl.S_Read(read_buf);
    printf("recv data of my world is :%s\n",read_buf.c_str());
    // if((nbytes=read(sockfd,buffer,1024))==-1)
    //{
// fprintf(stderr,"read error:%s\n",strerror(errno));
// exit(1);
    //  }
    // buffer[nbytes]='\0';
    // printf("I have received %s\n",buffer);
    close(sockfd);
    exit(0);
}