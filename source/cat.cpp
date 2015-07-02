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
    cgi << content_type_json;

    // args

    Args args{};

    RPP_META_DYNAMIC(
        "args", Args, VisitorListArgs
    ) args_meta{args};

    rpp::VisitorIStrTree<cgicc::FCgiCC<>> args_visitor{cgi};
    args_meta.doVisit(args_visitor);

    // find

    using namespace bsoncxx::builder::stream;

    auto cursor = db_cat.find(
        document{}
            << "_id" << args.cat_id << finalize
    );

    // return

    Cat result;

    RPP_META_DYNAMIC(
        "result", Cat, VisitorListDB
    ) result_meta{result};

    rpp::VisitorBSONView<> result_visitor{bsoncxx::types::b_document{*(cursor.begin())}};
    result_meta.doVisit(result_visitor);

    rpp::VisitorJSON<cgicc::FCgiCC<>> output{cgi};
    result_meta.doVisit(output);

    // for (auto &&doc: cursor) {
    //     cgi << bsoncxx::to_json(doc) << std::endl;
    // }
}

}

BOOKSTORE_MAIN(exec, ignoreErr)
