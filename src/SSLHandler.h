//
// Created by 苏畅 on 2019/7/3.
//

#ifndef NET_SAFE_SSLHANDLER_H
#define NET_SAFE_SSLHANDLER_H

#include<string>

#include "CA.h"

class  SSLContext {
private:
    long  _i_pub; // 个人的共钥匙
    long _i_pri; // 个人的私钥
    CA _t_ca; // 对方的共钥
    long _session_key; // 会话密钥
public:
    SSLContext(long i_pub, long i_pri, long t_pub) :_i_pri(i_pri), _i_pub(i_pub), _t_ca(t_pub), _session_key(-1)
    {}; //初始化

    long GetIPub(){
        return _i_pub;
    }

    long GetIPri(){
        return _i_pri;
    };

    long GetTPub(){
        return _i_pri;
    };

    long GetSessionKey() {
        return _session_key;
    }

    void SetSessionKey(long key) {
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

public:
    SSLHandler(int socketfd, SSLContext& ctx); // 传入一个链接好的socket进行构造

    int DoShakeHandsServer(); // 进行ssl握手

    // RESP: CA + '\r\n' +  CA str + '\r\n\r\n'
    // RESP: SESSION_KEY + '\r\n' + hash + '\r\n' + 'session_key 加密'  + '\r\n\r\n'

    int DoShakeHandsClient(); // 进行ssl手

    // REQ: HELLO
    // REQ: PUB + '\r\n' + hash + '\r\n' + PUBLIC_EKY
    // REQ: OK

    int S_Write(const std::string s); // 写数据 先对数据加密， 然后进行hash  消息格式：  哈希值长度 + '\r\n' + hash值  + \r\n + 加密后的消息 '\r\n\r\n'
    int S_Read(char *buf, int size); // 先对数据进行校验, 然后取出消息解密

    SSL_STATE GetSSLState() {
        return _state;
    }

};


#endif //NET_SAFE_SSLHANDLER_H
