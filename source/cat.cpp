#include "../header/bookstore.hpp"

namespace bookstore {

BOOKSTORE_DB_CONN()

struct Args {
    std::string cat_id;
};

RPP_TYPE_OBJECT(
    __(cat_id),
    Args
)

void exec(cgicc::FCgiCC<> &cgi) {
    BOOKSTORE_EXEC_ENTER(session, args)

    Cat result;
    dbGetOne(db_cat, result, args.cat_id);

    BOOKSTORE_EXEC_EXIT(result, session)
}

}

BOOKSTORE_MAIN(exec, ignoreErr)
