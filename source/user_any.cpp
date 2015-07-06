#include "../header/bookstore.hpp"

namespace bookstore {

BOOKSTORE_DB_CONN()

struct Args {
    std::string user_id;
};

RPP_TYPE_OBJECT(
    __(user_id),
    Args
)

void exec(cgicc::FCgiCC<> &cgi) {
    BOOKSTORE_EXEC_ENTER(session, args)

    // get data

    Subset<User> result;
    dbGetOne(db_user, result, args.user_id);

    BOOKSTORE_EXEC_EXIT(result, session)
}

}

BOOKSTORE_MAIN(exec, ignoreErr)
