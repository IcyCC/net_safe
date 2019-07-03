//
// Created by 苏畅 on 2019/7/3.
//

#ifndef NET_SAFE_CA_H
#define NET_SAFE_CA_H

#include <string>

class CA {
public:
    long _pub;
    std::string _sing;
public:
    std::string Dumps();
    void  Loads(const std::string &s); // _pub + \r\n+  _string
    void  Load(const std::string &path);
    void  Dump(const std::string &path);

    bool Check(const std::string &c_pub); //根据公钥校验
};

#endif //NET_SAFE_CA_H
