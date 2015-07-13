#include "bookstore_headers.hpp"

namespace wpp {

BOOKSTORE_DB_CONN()

struct Args {
    std::string mode;
    // long begin;
};

RPP_TYPE_OBJECT(
    __(mode),
    Args
)

struct Result {
    std::vector<Subset<Buy>> data;
};

RPP_TYPE_OBJECT(
    __(data),
    Result
)

void exec(cgicc::FCgiCC<> &cgi) {
    BOOKSTORE_EXEC_ENTER(session, args)

    // // find

    // using namespace bsoncxx::builder::stream;

    // auto cursor = db_buy.find(
    //     document{}
    //         << /* TODO */ << finalize
    // );

    // // get data

    Result result;

    // for (const auto &item: cursor) {
    //     result.data.push_back({});
    //     bsonGet(item, result.data.back());
    // }

    BOOKSTORE_EXEC_EXIT(result, session)
}

}

BOOKSTORE_MAIN(exec, ignoreErr)
