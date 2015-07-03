#include "../header/bookstore.hpp"

namespace bookstore {

BOOKSTORE_DB_CONN()

struct Args {
    std::string user_id;
    // long begin;
};

RPP_TYPE_OBJECT(
    __(user_id),
    Args
)

struct Result {
    std::vector<Subset<Book>> data;
};

RPP_TYPE_OBJECT(
    __(data),
    Result
)

void exec(cgicc::FCgiCC<> &cgi) {
    Args args;
    ajaxArgs(cgi, args);

    // find

    using namespace bsoncxx::builder::stream;

    auto cursor = db_book.find(
        document{}
            << "owner_user_id" << args.user_id << finalize
    );

    // get data

    Result result;

    for (const auto &item: cursor) {
        result.data.push_back({});
        dbGet(item, result.data.back());
    }

    ajaxReturn(cgi, makeSession(cgi, db_session), result);
}

}

BOOKSTORE_MAIN(exec, ignoreErr)
