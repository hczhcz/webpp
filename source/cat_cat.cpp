#include "../header/bookstore.hpp"

namespace bookstore {

BOOKSTORE_DB_CONN()

struct Args {
    std::string cat_id;
    // long begin;
};

RPP_TYPE_OBJECT(
    __(cat_id),
    Args
)

struct Result {
    std::vector<Subset<Cat>> data;
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

    auto cursor = db_cat.find(
        document{}
            << "parent_cat_id" << args.cat_id << finalize
    );

    // get data

    Result result;

    for (const auto &item: cursor) {
        result.data.push_back({});

        RPP_META_DYNAMIC(
            "item", Subset<Cat>, VisitorListDB
        ) result_meta{result.data.back()};

        rpp::VisitorBSONView<> result_visitor{bsoncxx::types::b_document{item}};
        result_meta.doVisit(result_visitor);
    }

    ajaxReturn(cgi, result);
}

}

BOOKSTORE_MAIN(exec, ignoreErr)
