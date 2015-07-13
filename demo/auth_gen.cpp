#include "bookstore_headers.hpp"

namespace wpp {

BOOKSTORE_DB_CONN()

struct Args {
};

RPP_TYPE_OBJECT(
    /* nothing */,
    Args
)

struct Result {
    std::string auth_salt;
};

RPP_TYPE_OBJECT(
    __(auth_salt),
    Result
)

void exec(cgicc::FCgiCC<> &cgi) {
    BOOKSTORE_EXEC_ENTER(session, args)

    std::string salt{randStr()};
    session.auth_salt = salt;

    Result result{salt};

    BOOKSTORE_EXEC_EXIT(result, session)
}

}

BOOKSTORE_MAIN(exec, ignoreErr)
