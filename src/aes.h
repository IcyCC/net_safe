#pragma once

#include <vector>
#include <string>


typedef std::vector<unsigned char> ByteArray;

#define BLOCK_SIZE 16

class Aes256 {

public:
    Aes256(const ByteArray& key);
    ~Aes256();

    static std::string byte2str_raw(const ByteArray & b);

    static ByteArray str2byte_raw(const std::string & b);


    static std::string encrypt(const std::string& key, const std::string& plain);
    static std::string decrypt(const std::string& key, const std::string& encrypted);

    long encrypt_start(const long plain_length, ByteArray& encrypted);
    long encrypt_continue(const ByteArray& plain, ByteArray& encrypted);
    long encrypt_continue(const unsigned char* plain, const long plain_length, ByteArray& encrypted);
    long encrypt_end(ByteArray& encrypted);

    long decrypt_start(const long encrypted_length);
    long decrypt_continue(const ByteArray& encrypted, ByteArray& plain);
    long decrypt_continue(const unsigned char* encrypted, const long encrypted_length, ByteArray& plain);
    long decrypt_end(ByteArray& plain);

private:
    ByteArray            m_key;
    ByteArray            m_salt;
    ByteArray            m_rkey;

    unsigned char        m_buffer[3 * BLOCK_SIZE];
    unsigned char        m_buffer_pos;
    long m_remainingLength;

    bool                 m_decryptInitialized;

    void check_and_encrypt_buffer(ByteArray& encrypted);
    void check_and_decrypt_buffer(ByteArray& plain);

    void encrypt(unsigned char *buffer);
    void decrypt(unsigned char *buffer);

    void expand_enc_key(unsigned char *rc);
    void expand_dec_key(unsigned char *rc);

    void sub_bytes(unsigned char *buffer);
    void sub_bytes_inv(unsigned char *buffer);

    void copy_key();

    void add_round_key(unsigned char *buffer, const unsigned char round);

    void shift_rows(unsigned char *buffer);
    void shift_rows_inv(unsigned char *buffer);

    void mix_columns(unsigned char *buffer);
    void mix_columns_inv(unsigned char *buffer);
};

