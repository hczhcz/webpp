#include "../header/bookstore.hpp"

namespace bookstore {

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
    std::vector<Subset2<Book>> data;
};

RPP_TYPE_OBJECT(
    __(data),
    Result
)

void exec(cgicc::FCgiCC<> &cgi) {
    BOOKSTORE_EXEC_ENTER(session, args)

    // find

    using namespace bsoncxx::builder::stream;

    mongocxx::options::find opts;
    opts.sort(
        args.mode == "new" ?
            document{} << "date_create" << -1 << finalize
        : args.mode == "sold" ?
            document{} << "sold_count" << -1 << finalize
        : // args.mode == "newsold" ?
            document{} << "sold_count" << -1 << finalize
    );

    auto cursor = (
        args.mode == "new" ?
            db_book.find({}, opts)
        : args.mode == "sold" ?
            db_book.find({}, opts)
        : // args.mode == "newsold" ?
            db_book.find(
                document{}
                << "date_create" << open_document
                    << "$lt" << (
                        time(nullptr) + 3600 * 24 * 30
                    ) << close_document << finalize,
                opts
            )
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
