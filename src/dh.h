//
// Created by 苏畅 on 2019/5/22.
//

#ifndef NET_SAFE_DH_H
#define NET_SAFE_DH_H

#include"util.h"

class DHMan {
public:
    int q; //
    int a;
    int y;
    int x;
public:
    DHMan(int _x): x(_x) {
        q = 13;
        a = 2;
        computeY();
    };

    DHMan(int _x, int _q, int _a):x(_x),q(_q), a(_a) {
        computeY();
    }

    void computeY() {
        y = int(pow(a, x)) % q;
    };

    int GetKey(int yb) {
        return int(pow(yb,x)) % q;
    }

    int GetY(){
        return y;
    }
};

#endif //NET_SAFE_DH_H
