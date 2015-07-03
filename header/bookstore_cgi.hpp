#pragma once

#include <cgicc/HTTPContentHeader.h>
#include <cgicc/HTTPCookie.h>

#include "cgiutil.hpp"

namespace bookstore {

const char content_type_text[]{
    "text/plain; charset=utf-8"
};
const char content_type_json[]{
    "application/json; charset=utf-8"
};

const char session_tag_id[]{"BOOKSTORE_SESSION_ID"};
const char session_tag_key[]{"BOOKSTORE_SESSION_KEY"};

inline void ignoreErr(const std::exception &) {
    // ignore
}

}
