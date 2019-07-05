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


inline bool IsEndWith(const std::string &s, const std::string &m) {
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


inline std::string RandStr(int len) {
    char str[len + 1];
    bzero(str, len+1);
    int i, flag;
    srand(time(NULL));//通过时间函数设置随机数种子，使得每次运行结果随机。
    for (i = 0; i < len; i++) {
        flag = rand() % 3;
        switch (flag) {
            case 0:
                str[i] = rand() % 26 + 'a';
                break;
            case 1:
                str[i] = rand() % 26 + 'A';
                break;
            case 2:
                str[i] = rand() % 10 + '0';
                break;
        }
    }
    str[i] = '\0';
    return str;
}

std::vector<std::string> split_string(std::string src, std::string sp);

#endif //NET_SAFE_UTIL_H
