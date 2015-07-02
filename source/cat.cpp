#include "../header/bookstore.hpp"

namespace bookstore {

struct Args {
    std::string cat_id;
};

RPP_TYPE_OBJECT(
    __(cat_id),
    Args
)

void exec(cgicc::FCgiCC<> &cgi) {
    cgi << "Content-Type: application/json; charset=utf-8;\r\n\r\n";

    // args

    Args args{};

    RPP_META_DYNAMIC(
        "args", Args, rpp::TypeList<rpp::VisitorIStrTree<cgicc::FCgiCC<>>>
    ) args_meta{args};

    rpp::VisitorIStrTree<cgicc::FCgiCC<>> v_arg{cgi};
    args_meta.doVisit(v_arg);

    // db access

    mongocxx::instance inst{};
    mongocxx::client conn{};

    auto db = conn["bookstore"];
    auto db_cat = db["cat"];

    // find

    using namespace bsoncxx::builder::stream;

    auto cursor = db_cat.find(
        document{}
            << "_id" << args.cat_id << finalize
    );

    for (auto &&doc: cursor) {
        cgi << bsoncxx::to_json(doc) << std::endl;
    }

    // rpp::VisitorJSON<cgicc::FCgiCC<>> v_json{cgi};
    // args_meta.doVisit(v_json);
}

void err(const std::exception &) {
    // ignore
}

}

int main() {
    fcgicc_exec(bookstore::exec, bookstore::err);

    return 0;
}
