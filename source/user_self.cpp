#include "../header/bookstore.hpp"

namespace bookstore {

BOOKSTORE_DB_CONN()

struct Args {
};

RPP_TYPE_OBJECT(
    /* nothing */,
    Args
)

void exec(cgicc::FCgiCC<> &cgi) {
    BOOKSTORE_EXEC_ENTER(session, args)

    std::string user_id{session.auth_user_id.force()};

    // get data

    User result;
    dbGetOne(db_user, result, user_id);

    BOOKSTORE_EXEC_EXIT(result, session)
}

}

BOOKSTORE_MAIN(exec, ignoreErr)
