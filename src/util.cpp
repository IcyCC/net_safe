//
// Created by 苏畅 on 2019/6/11.
//

#include "util.h"

std::vector<std::string> split_string(std::string src, std::string sp) {
    std::string::size_type pos1, pos2 = 0;
    std::vector<std::string> v;
    pos2 = src.find(sp);
    pos1 = 0;
    while (std::string::npos != pos2) {
        v.push_back(src.substr(pos1, pos2 - pos1));

        pos1 = pos2 + sp.size();
        pos2 = src.find(sp, pos1);
    }
    if (pos1 != src.length()) {
        v.push_back(src.substr(pos1));
    }
    return v;
}

