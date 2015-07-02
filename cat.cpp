#include "reflection++/meta.hpp"

#include "bookstore_model.hpp"
#include "bookstore_cgi.hpp"

namespace cgicc {

RPP_VISITOR_CHAIN_INIT()
RPP_VISITOR_REG(rpp::VisitorIStrTree<FCgiCC<>>)
RPP_VISITOR_REG(rpp::VisitorJSON<FCgiCC<>>)
RPP_VISITOR_COLLECT(VisitorList)

}

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

    Args args{};

    RPP_META_DYNAMIC(
        "args", Args, cgicc::VisitorList
    ) args_meta{args};

    rpp::VisitorIStrTree<cgicc::FCgiCC<>> v_arg{cgi};
    args_meta.doVisit(v_arg);

    rpp::VisitorJSON<cgicc::FCgiCC<>> v_json{cgi};
    args_meta.doVisit(v_json);
}

void err(const std::exception &) {
    // ignore
}

}

int main() {
    fcgicc_exec(bookstore::exec, bookstore::err);

    return 0;
}
