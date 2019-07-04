//
// Created by 苏畅 on 2019/7/3.
//

#include "SSLHandler.h"
#include "Codec.h"
#include <string>
#include <iostream>
#include<sys/socket.h>
#include<cstdio>
#include <sys/fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "util.h"
#include "aes.h"
#include "rsa.h"
#include "sha512.h"

SSLHandler::SSLHandler(int socketfd, const std::string &pub, const std::string &pri, const std::string ca_pub, CA &i_ca) {
    _socket_sfd = socketfd;
    _ctx._i_ca = i_ca;
    _ctx._i_pri = pri;
    _ctx._ca_pub = ca_pub;
    _state = SSL_STATE ::IDLE;
}

int SSLHandler::DoShakeHandsClient() {
    _state = SSL_STATE ::SHAKEHANDS;
    auto hello = _codec.encode("HELLO");//发送hello
    ::write(_socket_sfd, hello.c_str(), hello.length());
    std::string server_ca_resp;
    std::string res;
    char buf[1024];
    bzero(buf, sizeof(buf));
    while(true){
        int flag = ::read(_socket_sfd, buf, sizeof(buf));
        if (flag < 0 && errno != EINTR && errno != EAGAIN){
            return  -1;
        }
        res.append(buf);
        bzero(buf, sizeof(buf));
        server_ca_resp = _codec.tryDecode(res);
        if(!server_ca_resp.empty()){
            break;
        }
    }

    // 校验buffer
    auto res_ca_info = split_string(server_ca_resp, "\r\n");
    if (res_ca_info[0] != "CA"){
        std::cout<<"非法参数"<<std::endl;
        return -1;
    }
    _ctx._t_ca.Loads(res_ca_info[1]);
    if (!_ctx._t_ca.Check(_ctx._ca_pub)) {
        // ca证书未签名
        std::cout<<"ca未签名"<<std::endl;
        return -1;
    }

    auto self_ca = _codec.encode("CA\r\n"+_ctx._i_ca.Dumps());
    ::write(_socket_sfd, self_ca.data(), self_ca.length()); // 发送自己的证书
    std::string session_key_resp;

    // 准备接受会话密钥
    bzero(buf, sizeof(buf));
    while(true){
        int flag = ::read(_socket_sfd, buf, sizeof(buf));
        if (flag<0 && errno != EINTR && errno != EAGAIN){
            return  -1;
        }
        res.append(buf);
        bzero(buf, sizeof(buf));
        session_key_resp = _codec.tryDecode(res);
        if(!session_key_resp.empty()){
            break;
        }
    }
    auto session_key_info = split_string(session_key_resp, "\r\n");
    if (session_key_info[0] != "SESSION_KEY") {
        std::cout<<"非法参数"<<std::endl;
        return  -1;
    }
    auto session_key = rsa_decrypt(session_key_info[2], _ctx._i_pri);
    // 校验签名
    auto session_key_sign = session_key_info[1];
    if (rsa_decrypt(session_key_sign, _ctx._t_ca._pub) != session_key) {
        std::cout<<"签名不通过"<<std::endl;
        return  -1;
    }
    _ctx._session_key = session_key;
    // 会话密钥接受完成

    //发送ok
    auto ok_req = _codec.encode("OK");
    ::write(_socket_sfd,ok_req.data(),ok_req.length());
    _state = SSL_STATE ::TRANS;
    return 1;
}


int SSLHandler::DoShakeHandsServer() {
    _state = SSL_STATE ::SHAKEHANDS;
    std::string hello_resp;
    std::string res;
    char buf[1024];
    bzero(buf, sizeof(buf));
    while(true){
        int flag = ::read(_socket_sfd, buf, sizeof(buf));
        if (flag<0&&errno != EINTR && errno != EAGAIN){
            return  -1;
        }
        res.append(buf);
        bzero(buf, sizeof(buf));
        hello_resp = _codec.tryDecode(res);
        if(!hello_resp.empty()){
            break;
        }
    }
    auto hello_resp_info = split_string(hello_resp, "\r\n");
    if(hello_resp_info[0] != "HELLO") {
        std::cout<<"非法参数"<<std::endl;
        return  -1;
    }

    auto server_ca_resp = _codec.encode("CA\r\n" + _ctx._i_ca.Dumps());
    ::write(_socket_sfd,server_ca_resp.data(), server_ca_resp.length());

    // 接受客户端ca
    std::string client_ca_resp;
    bzero(buf, sizeof(buf));
    while(true){
        int flag = ::read(_socket_sfd, buf, sizeof(buf));
        if (flag<0){
            return  -1;
        }
        res.append(buf);
        bzero(buf, sizeof(buf));
        client_ca_resp = _codec.tryDecode(res);
        if(!client_ca_resp.empty()){
            break;
        }
    }
    auto client_ca_info = split_string(client_ca_resp, "\r\n");
    if(client_ca_info[0] != "CA" ){
        std::cout<<"非法参数"<<std::endl;
        return -1;
    }
    _ctx._t_ca.Loads(client_ca_info[1]);

    //会话密钥
    _ctx._session_key = "hello";
    auto session_key = rsa_encrypt(_ctx._session_key, _ctx._t_ca._pub);
    auto session_key_sign = rsa_encrypt(session_key, _ctx._i_pri);
    auto session_key_resp = _codec.encode(std::string("SESSION_KEY\r\n")+session_key_sign+"\r\n"+session_key);
    ::write(_socket_sfd, session_key_resp.data(), session_key_resp.length());

    // 接受ok
    std::string ok_resp;
    bzero(buf, sizeof(buf));
    while(true){
        int flag = ::read(_socket_sfd, buf, sizeof(buf));
        if (flag<0){
            return  -1;
        }
        res.append(buf);
        bzero(buf, sizeof(buf));
        ok_resp = _codec.tryDecode(res);
        if(!ok_resp.empty()){
            break;
        }
    }

    auto ok_resp_info = split_string(ok_resp, "\r\n");
    if (ok_resp_info[0] != "OK"){
        std::cout<<"非法参数"<<std::endl;
        return -1;
    }
    _state = SSL_STATE ::TRANS;
    return  1;

}

int SSLHandler::S_Read(std::string &buf) {
    char buffer[1024];
    bzero(buffer, sizeof(buffer));
    while (true) {
        int flag = ::read(_socket_sfd, buffer, sizeof(buffer));
        if (flag <= 0 && errno != EINTR && errno != EAGAIN) {
            break;
        }
        _buffer.append(buffer);
        bzero(buffer, sizeof(buffer));
    }
    std::string buffer_part = _codec.tryDecode(_buffer);
    while (buffer_part != "") {
        std::vector<std::string> parts = split_string(buffer_part, "\r\n");
        std::string words = Aes256::decrypt(parts[2], _ctx._session_key);
        std::string words_hash = getHash(words.c_str());
        if(words_hash != parts[1]) {
            return -1;
        }
        buf.append(words);
        buf.append("\r\n\r\n");
    }
}



std::string GenMsg(const std::string hash,const std::string s_data)
{
    std::string msg="64\r\n";
    msg.append(hash);
    msg.append("\r\n");
    msg.append(s_data);
    msg.append("\r\n\r\n");
    return msg;
}


int SSLHandler::S_Write(const std::string s) {
    uint8_t hashdata[64];
    std::string hash=getHash(s.c_str());

    std::string key=this->_ctx.GetSessionKey();
    auto c = Aes256::encrypt(key, s);

    std::string target=GenMsg(hash,c); /*生成消息格式*/

    write(this->_socket_sfd,target.c_str(), target.length());
    return target.length();
} // 写数据 先对数据， 然后进行hash  消息格式：  哈希值长度 + '\r\n' +加密前hash值  + \r\n + 加密后的消息 '\r\n\r\n'