#pragma once

#include "cgiutil.hpp"

namespace bookstore {

const char content_type_text[]{
    "Content-Type: text/plain; charset=utf-8;\r\n\r\n"
};
const char content_type_json[]{
    "Content-Type: application/json; charset=utf-8;\r\n\r\n"
};

inline void ignoreErr(const std::exception &) {
    // ignore
}

}
