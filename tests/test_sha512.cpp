
#include <stdint.h>
#include <iostream>
#include <string.h>
#include "../src/sha512.h"
using namespace std;


int main()
{
    std::string a = "hhhhheeeee";
    std::string h1=getHash(a.c_str());
    std::string h2=getHash(a.c_str());

    return 0;
}