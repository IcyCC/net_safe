#include <string>


std::string rsa_encrypt(std::string &input,  std::string public_key);
std::string rsa_decrypt(std::string &input,  std::string private_key);
void get_public_key(std::string &public_key, std::string &private_key);
