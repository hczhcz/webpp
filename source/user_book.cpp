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
    BOOKSTORE_EXEC_ENTER(session, args)

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

    BOOKSTORE_EXEC_EXIT(result, session)
}

}

BOOKSTORE_MAIN(exec, ignoreErr)
