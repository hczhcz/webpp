#include "../header/bookstore.hpp"

namespace bookstore {

BOOKSTORE_DB_CONN()

struct Args {
};

RPP_TYPE_OBJECT(
    /* nothing */,
    Args
)

struct Result {
    Maybe<std::string> auth_user_id;
    Maybe<std::string> auth_name;
    bool auth_sudo;
};

RPP_TYPE_OBJECT(
    __(auth_user_id) __(auth_name) __(auth_sudo),
    Result
)

void exec(cgicc::FCgiCC<> &cgi) {
    BOOKSTORE_EXEC_ENTER(session, args)

    Result result;

    if (session.auth_user_id) {
        User user;
        dbGetOne(db_user, user, session.auth_user_id.force());

        result = Result{
            session.auth_user_id.force(),
            session.auth_name.force(),
            session.auth_sudo
        };
    } else {
        result = Result{nullptr, nullptr, false};
    }

    BOOKSTORE_EXEC_EXIT(result, session)
}

}

BOOKSTORE_MAIN(exec, ignoreErr)
