cmake_minimum_required(VERSION 3.12)
project(net_safe)

set(CMAKE_CXX_STANDARD 14)

add_executable(net_safe
        src/dh.cpp
        src/dh.h
        src/util.cpp
        src/util.h
        src/SSLHandler.cpp
        src/SSLHandler.h
        src/CA.cpp
        src/CA.h
        src/aes.cpp
        src/aes.h
        src/rsa.cpp
        src/des.cpp
        src/rsa.h
        src/sha512.cpp
        src/sha512.h
        src/Codec.h
        src/Codec.cpp
        tests/ssl_client.cpp
        #tests/ssl_server.cpp
        #tests/make_ca.cpp
        #tests/test_rsa.cpp
        tests/test_ca.cpp
        )


