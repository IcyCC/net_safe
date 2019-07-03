//
// Created by 苏畅 on 2019/7/3.
//

#ifndef NET_SAFE_CODEC_H
#define NET_SAFE_CODEC_H

#include <string>
#include "util.h"

struct LineCodec {
    std::string tryDecode(std::string &data);

    std::string encode(const std::string &data);
};


#endif //NET_SAFE_CODEC_H
