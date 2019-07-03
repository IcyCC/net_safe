//
// Created by 苏畅 on 2019/7/3.
//

#ifndef NET_SAFE_CA_H
#define NET_SAFE_CA_H

#include <string>
#include "util.h"

class CA {
public:
    long _pub;
    std::string _sing;
public:
    CA() {};

    CA(long _pub, std::string _sing) : _pub(_pub), _sing(_sing) {};

    std::string Dumps();

    void Loads(const std::string &s); // _pub + \r\n+  _string
    void Load(const std::string &path);

    void Dump(const std::string &path);

    bool Check(const std::string &c_pub); //根据公钥校验
};

#endif //NET_SAFE_CA_H
