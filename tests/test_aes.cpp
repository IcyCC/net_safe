//
// Created by 苏畅 on 2019/7/3.
//

#include "../src/aes.h"

#include <vector>
#include <string>
#include <iostream>

int main() {
    std::string key = "hello1";
    std::string raw = "HELLO1";
    auto c = Aes256::encrypt(key, raw);
    auto c2 = Aes256::encrypt("hello1", "HELLO1");
    auto res2 = Aes256::decrypt(key, c);
    return 0;
}

