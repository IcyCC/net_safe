//
// Created by 苏畅 on 2019/7/3.
//

#include "CA.h"


std::string CA::Dumps() {
    return _pub + "_" + _sing;
}

void CA::Loads(const std::string &s) {
    std::vector<std::string> parts = split_string(s, "_");
    std::string pub  = parts[0]+"_"+parts[1];
    std::string sing = parts[2];
    _pub = pub;
    _sing = sing;
}

bool CA::Check(const std::string &c_pub) {
    std::string pub_may = rsa_decrypt(_sing, c_pub);
    if(pub_may==_pub) {
        return true;
    } else {
        return false;
    }
}