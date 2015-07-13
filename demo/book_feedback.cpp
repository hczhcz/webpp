#include "bookstore_headers.hpp"

namespace wpp {

BOOKSTORE_DB_CONN()

struct Args {
    std::string book_id;
    // long begin;
};

RPP_TYPE_OBJECT(
    __(book_id),
    Args
)

struct Result {
    std::vector<Subset2<Buy>> data;
};

RPP_TYPE_OBJECT(
    __(data),
    Result
)

void exec(cgicc::FCgiCC<> &cgi) {
    BOOKSTORE_EXEC_ENTER(session, args)

    // find

    using namespace bsoncxx::builder::stream;

    auto cursor = db_buy.find(
        document{}
            << "buy_book_id" << args.book_id << finalize
    );

    // get data

    Result result;

    for (const auto &item: cursor) {
        result.data.push_back({});
        bsonGet(item, result.data.back());
    }

    BOOKSTORE_EXEC_EXIT(result, session)
}

}

BOOKSTORE_MAIN(exec, ignoreErr)
