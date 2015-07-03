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
    Args args;
    ajaxArgs(cgi, args);

    // find

    using namespace bsoncxx::builder::stream;

    auto cursor = db_cat.find(
        document{}
            << "_id" << args.cat_id << finalize
    );

    // get data

    Cat result;
    dbGet(*cursor.begin(), result);

    ajaxReturn(cgi, result);
}

}

BOOKSTORE_MAIN(exec, ignoreErr)