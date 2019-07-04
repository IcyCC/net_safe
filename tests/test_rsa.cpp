//
// Created by zhandong on 2019-07-04.
//
#include "../src/rsa.h"
#include <string>
#include <fstream>
#include <iostream>



int main(){
    std::string ca_public_key, ca_private_key;
    get_public_key(ca_public_key, ca_private_key);
    std::string input = "test";
    std::string result = rsa_encrypt(input, ca_public_key);
    std::string output = rsa_decrypt(result, ca_private_key);
    std::cout<<output<<std::endl;
}