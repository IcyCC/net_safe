//
// Created by 苏畅 on 2019/7/3.
//

#include "../src/aes.h"

#include <vector>
#include <string>
#include <iostream>

int main() {
    std::string key = "1234";
    std::string raw = "hello";
    auto c = Aes256::encrypt(key, raw);
    auto res = Aes256::decrypt(key, c);
    return 0;
}