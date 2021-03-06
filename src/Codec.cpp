//
// Created by 苏畅 on 2019/7/3.
//

#include "Codec.h"
#include <string>


std::string LineCodec::tryDecode(std::string &data) {
    std::string res;
    long flag = 0;
    for(auto i : data) {
        res.push_back(i);
        if (IsEndWith(res,  "\r\n\r\n")) {
            data = data.substr(flag+1);
            res.pop_back();
            res.pop_back();
            return res;
        }
        flag++;
    }
    return "";
}

std::string LineCodec::encode(const std::string &data) {
    return data + "\r\n\r\n";
};
