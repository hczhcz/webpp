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
    Args args;
    ajaxArgs(cgi, args);

    // find

    using namespace bsoncxx::builder::stream;

    auto cursor = db_book.find(
        document{}
            << "_id" << args.book_id << finalize
    );

    // get data

    Book result;

    RPP_META_DYNAMIC(
        "result", Book, VisitorListDB
    ) result_meta{result};

    rpp::VisitorBSONView<> result_visitor{
        bsoncxx::types::b_document{*(cursor.begin())}
    };
    result_meta.doVisit(result_visitor);

    ajaxReturn(cgi, result);
}

}

BOOKSTORE_MAIN(exec, ignoreErr)
