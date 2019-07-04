//
// Created by zhandong on 2019-07-04.
//
#include "../src/rsa.h"
#include <string>
#include <fstream>



int WriteString2FileAppend(const std::string &file_string, const std::string& str)
{
    std::ofstream OsWrite(file_string, std::ofstream::app);
    OsWrite << str;
    OsWrite << std::endl;
    return 0;
}


int main(){
    std::string ca_public_key, ca_private_key;
    get_public_key(ca_public_key, ca_private_key);
    WriteString2FileAppend("../keys.txt", ca_public_key);
    WriteString2FileAppend("../keys.txt", ca_private_key);
    WriteString2FileAppend("../keys.txt", "");
    for(int i=0;i<2;i++) {
        std::string public_key, private_key;
        get_public_key(public_key, private_key);
        WriteString2FileAppend("../keys.txt", public_key);
        WriteString2FileAppend("../keys.txt", private_key);
        WriteString2FileAppend("../keys.txt", "");
        std::string result = rsa_encrypt(public_key, ca_private_key);
        WriteString2FileAppend("../keys.txt", public_key +"_"+ result);
        WriteString2FileAppend("../keys.txt", "");
    }

}