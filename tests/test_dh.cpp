//
// Created by 苏畅 on 2019/5/22.
//

#include"../src/dh.h"
#include <iostream>

int main() {
    auto d1 = DHMan(5);
    auto d2 = DHMan(10);

    int k1 = d1.GetKey(d2.GetY());
    int k2 = d2.GetKey(d1.GetY());

    std::cout<<"D1 Y: "<<d1.GetY()<<" D1 KEY: " << k1<<std::endl;
    std::cout<<"D2 Y: "<<d2.GetY()<<" D2 KEY: " << k2<<std::endl;

    assert(k1 == k2);

    return 0;
}