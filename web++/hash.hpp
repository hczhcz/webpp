#pragma once

#include <bsoncxx/oid.hpp>

#include "hashutil.hpp"

namespace wpp {

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
