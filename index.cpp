#include "reflection++/visitor_chain.hpp"
#include "reflection++/visitor/strtree.hpp"
#include "reflection++/visitor/json.hpp"
#include "reflection++/accessor_infer.hpp"
#include "reflection++/meta.hpp"

#include "cgiutil.hpp"

#include "cgicc/HTTPContentHeader.h"
#include "cgicc/HTMLClasses.h"

namespace demo {

RPP_VISITOR_CHAIN_INIT()
RPP_VISITOR_REG(rpp::VisitorIStrTree<cgicc::FCgiCC<> &>)
RPP_VISITOR_REG(rpp::VisitorJSON<cgicc::FCgiCC<>>)
RPP_VISITOR_COLLECT(VisitorList)

RPP_ACCESSOR_INFER_INIT()

struct Person {
    std::string name;
    int age;
};

RPP_TYPE_OBJECT(
    __(name)
    __(age),
    Person
)

void exec(cgicc::FCgiCC<> &cgi) {
    // cgi << cgicc::HTTPContentHeader("application/json");
    cgi << "Content-Type: application/json; charset=utf-8;\r\n\r\n";

    Person person{"hcz", 20};

    rpp::MetaImpl<
        VisitorList,
        RPP_ACCESSOR_GET_AS("person", DYNAMIC, Person)
    > meta{person};

    rpp::VisitorIStrTree<cgicc::FCgiCC<> &> v_arg{cgi};
    meta.doVisit(v_arg);
    rpp::VisitorJSON<cgicc::FCgiCC<>> v_json{cgi};
    meta.doVisit(v_json);
}

void err(const std::exception &) {
    // ignore
}

}

int main() {
    fcgicc_exec(demo::exec, demo::err);

    return 0;
}
