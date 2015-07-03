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

    // find

    using namespace bsoncxx::builder::stream;

    auto cursor = db_user.find(
        document{}
            << "_id" << args.user_id << finalize
    );

    // get data

    Subset<User> result;
    dbGet(*cursor.begin(), result);

    BOOKSTORE_EXEC_EXIT(result, session)
}

}

BOOKSTORE_MAIN(exec, ignoreErr)
