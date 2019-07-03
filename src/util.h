//
// Created by 苏畅 on 2019/6/11.
//

#ifndef NET_SAFE_UTIL_H
#define NET_SAFE_UTIL_H
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>


class Util {
};


inline bool IsEndWith(const std::string & s, const std::string& m) {
    if (m.length() > s.length()) {
        return false;
    }
    for (int i = 0; i < m.length(); i++) {
        if (m[i] != s[s.length() - m.length() + i]) {
            return false;
        }
    }
    return true;
}


std::vector<std::string> split_string(std::string src, std::string sp);
#endif //NET_SAFE_UTIL_H
