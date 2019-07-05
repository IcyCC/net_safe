//
// Created by 苏畅 on 2019/7/5.
//

#include "../src/sha512.h"
#include "../src/rsa.h"

int main() {
    auto session_key = "hello";
    auto _pri = "33455_131669";
    auto _pub = "5327_131669";

    auto s = rsa_encrypt(session_key, _pri);
    auto a = rsa_decrypt(s, _pub);
    return 0;
}