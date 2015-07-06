#include "../header/bookstore.hpp"

namespace bookstore {

BOOKSTORE_DB_CONN()

struct Args {
    std::string book_id;
};

RPP_TYPE_OBJECT(
    __(book_id),
    Args
)

void exec(cgicc::FCgiCC<> &cgi) {
    BOOKSTORE_EXEC_ENTER(session, args)

    // get data

    Book result;
    dbGetOne(db_book, result, args.book_id);

    BOOKSTORE_EXEC_EXIT(result, session)
}

}

BOOKSTORE_MAIN(exec, ignoreErr)
