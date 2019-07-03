//
// Created by 苏畅 on 2019/7/3.
//

#include "SSLHandler.h"
#include<sys/socket.h>
#include<cstdio>
#include <sys/fcntl.h>
#include <unistd.h>
#include "util.h"

int SSLHandler::DoShakeHandsClient() {
    auto hello = _codec.encode("Hello");//发送hello
    ::write(_socket_sfd, hello.c_str(), hello.length());
    std::string server_ca_resp;
    std::string res;
    char buf[1024];
    bzero(buf, sizeof(1024));
    while(true){
        ::read(_socket_sfd, buf, sizeof(buf));
        res  = res + buf;
        bzero(buf, sizeof(buf));
        server_ca_resp = _codec.tryDecode(res);
        if(server_ca_resp!= ""){
            break;
        }
    }

    // 校验buffer
    auto res_ca_info = split_string(server_ca_resp, "\r\n");
    if (res_ca_info[0] != "CA"){
        return -1;
    }
    CA server_ca;
    server_ca.Loads(res_ca_info[1]);
    server_ca.
}
