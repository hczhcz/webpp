#pragma once

#include <openssl/rand.h>
#include <openssl/sha.h>
#include <bsoncxx/oid.hpp>

#include <string>

namespace wpp {

template <class Output = std::string>
Output randStr() {
    unsigned char buf[64];

    RAND_bytes(buf, 64);

    Output result;
    for (long i = 0; i < 64; ++i) {
        result += "0123456789abcdef"[(buf[i] >> 4) & 0xf];
        result += "0123456789abcdef"[buf[i] & 0xf];
    }

    return result;
}

template <class Input, class Output = std::string>
Output hashStr(const Input &value) {
    unsigned char buf[SHA256_DIGEST_LENGTH];

    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, value.data(), value.size());
    SHA256_Final(buf, &sha256);

    Output result;
    for (long i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        result += "0123456789abcdef"[(buf[i] >> 4) & 0xf];
        result += "0123456789abcdef"[buf[i] & 0xf];
    }

    return result;
}

inline std::string saltedHash(
    const std::string &value,
    const std::string &salt
) {
    return hashStr(value + salt);
}

inline std::string passwordHash(
    const std::string &name,
    const std::string &password
) {
    return hashStr(name + ':' + password);
}

inline std::string genOID() {
    return bsoncxx::oid{bsoncxx::oid::init_tag}.to_string();
}

}
