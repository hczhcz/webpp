#include "../header/bookstore.hpp"

namespace bookstore {

BOOKSTORE_DB_CONN()

struct Args {
    //
};

RPP_TYPE_OBJECT(
    // __(???),
    ,
    Args
)

void exec(cgicc::FCgiCC<> &cgi) {
    Args args;
    ajaxArgs(cgi, args);

    // action

    // find

    using namespace bsoncxx::builder::stream;

    // auto cursor = db_???.find(
    //     document{}
    //         << "_id" << ??? << finalize
    // );

    // get data

    // ??? result;
    int result{0};

    // RPP_META_DYNAMIC(
    //     "result", ???, VisitorListDB
    // ) result_meta{result};

    // rpp::VisitorBSONView<> result_visitor{bsoncxx::types::b_document{*(cursor.begin())}};
    // result_meta.doVisit(result_visitor);

    ajaxReturn(cgi, result);
}

}

BOOKSTORE_MAIN(exec, ignoreErr)
