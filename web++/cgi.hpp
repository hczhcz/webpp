#pragma once

#include <cgicc/HTTPContentHeader.h>
#include <cgicc/HTTPCookie.h>

#include "cgiutil.hpp"

namespace wpp {

const char content_type_text[]{
    "text/plain; charset=utf-8"
};
const char content_type_json[]{
    "application/json; charset=utf-8"
};

const char session_tag_id[]{"WPP_SESSION_ID"};
const char session_tag_key[]{"WPP_SESSION_KEY"};

// execute a FastCGI (+CGICC) event loop
template <class Env, class In, class Out, class Err, class E>
void fcgiccExec(
    void (&func)(cgicc::FCgiCC<Env, In, Out, Err> &),
    void (&except)(const E &)
) {
    FCGX_Request request;

    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);

    while (FCGX_Accept_r(&request) == 0) {
        try {
            cgicc::FCgiCC<Env, In, Out, Err> cgi{
                request, std::cin, std::cout, std::cerr // TODO: create new stream
            };

            func(cgi);
        } catch (const E &e) {
            except(e);
        }
    }
}

// simple request error handler
inline void ignoreErr(const std::exception &) {
    // ignore
    // TODO: print error log? backtrace?
}

}
