#include "bookstore_headers.hpp"

namespace wpp {

BOOKSTORE_DB_CONN()

struct Args {
    std::string login_user_id;
};

RPP_TYPE_OBJECT(
    __(login_user_id),
    Args
)

struct Result {
    bool auth_success;
};

RPP_TYPE_OBJECT(
    __(auth_success),
    Result
)

void exec(cgicc::FCgiCC<> &cgi) {
    BOOKSTORE_EXEC_ENTER(session, args)

    std::string user_id{session.auth_user_id.force()};

    Result result;

    if (user_id == args.login_user_id) {
        // logout ok

        session.auth_salt = nullptr;
        session.auth_user_id = nullptr;
        session.auth_name = nullptr;
        session.auth_sudo = false;

        result = Result{true};
    } else {
        // wrong id

        result = Result{false};
    }

    BOOKSTORE_EXEC_EXIT(result, session)
}

}

BOOKSTORE_MAIN(exec, ignoreErr)
