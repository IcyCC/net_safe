#include <string>


std::string rsa_encrypt(const std::string &input, const std::string& public_key);
std::string rsa_decrypt(const std::string &input, const std::string& public_key);
void get_public_key( std::string &public_key, std::string &private_key);
