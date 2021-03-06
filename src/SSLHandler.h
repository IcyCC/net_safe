//
// Created by 苏畅 on 2019/7/3.
//

#ifndef NET_SAFE_SSLHANDLER_H
#define NET_SAFE_SSLHANDLER_H

#include<string>

#include "CA.h"
#include "Codec.h"

class  SSLContext {
public:
    std::string _ca_pub;
    std::string _i_pri; // 个人的私钥
    CA _t_ca; // 对方的证书
    CA _i_ca; // 自己的证书
    std::string _session_key; // 会话密钥
public:
    SSLContext(const std::string& i_pub, const std::string& i_pri, CA &t_ca) :_i_pri(i_pri),  _t_ca(t_ca), _session_key("")
    {}; //初始化

    SSLContext(){};


    std::string GetIPri(){
        return _i_pri;
    };

    std::string GetTPub(){
        return _i_pri;
    };

    std::string GetSessionKey() {
        return _session_key;
    }

    void SetSessionKey(const std::string &key) {
        _session_key = key;
    }

};

class SSLHandler {

public:
    enum SSL_STATE {
        IDLE,
        SHAKEHANDS,
        TRANS,
        CLOSE
    };
private:
    SSL_STATE _state;
    SSLContext _ctx;
    int _socket_sfd;

    std::string _buffer;

    LineCodec _codec;
public:
    SSLHandler(int socketfd, const std::string & pub, const std::string& pri, const  std::string ca_pub , CA & i_ca); // 传入一个链接好的socket进行构造

    int DoShakeHandsServer(); // 进行ssl握手

    // RESP: CA + '\r\n' +  CA str + '\r\n\r\n'
    // RESP: SESSION_KEY + '\r\n' + 加密前的hash + '\r\n' + 'session_key 加密'  + '\r\n\r\n'

    int DoShakeHandsClient(); // 进行ssl手

    // REQ: HELLO + '\r\n\r\n'
    // REQ:  CA + '\r\n' +  CA str + '\r\n\r\n'
    // REQ: OK

    int S_Write(const std::string s); // 写数据 先对数据， 然后进行hash  消息格式：  哈希值长度 + '\r\n' +加密前hash值  + \r\n + 加密后的消息 '\r\n\r\n'
    int S_Read(std::string &buf); // 先对, 然后取出消息解密 再 数据进行校验

    SSL_STATE GetSSLState() {
        return _state;
    }

};


#endif //NET_SAFE_SSLHANDLER_H
