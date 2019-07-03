//
// Created by 苏畅 on 2019/7/3.
//

#include "CA.h"


std::string CA::Dumps() {
    return std::to_string(_pub) + "_" + _sing;
}

void CA::Loads(const std::string &s) {
    std::vector<std::string> parts = split_string(s, "_");
    long pub  = atol(parts[0].c_str());
    std::string sing = parts[1];
    _pub = pub;
    _sing = sing;
}

bool CA::Check(const std::string &c_pub) {

}