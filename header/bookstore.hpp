#pragma once

#include <bsoncxx/oid.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/find.hpp>

#include "reflection++/visitor/strtree.hpp"
#include "reflection++/visitor/json.hpp"
#include "reflection++/visitor/bson.hpp"
#include "reflection++/visitor/bson_view.hpp"
#include "reflection++/meta.hpp"

#include "bookstore_model.hpp"
#include "bookstore_cgi.hpp"

namespace bookstore {

// visitors

using VisitorListArgs = rpp::TypeList<
    rpp::VisitorIStrTree<cgicc::FCgiCC<>>
>;
using VisitorListJSON = rpp::TypeList<
    rpp::VisitorJSON<cgicc::FCgiCC<>>
>;
using VisitorListDB = rpp::TypeList<
    rpp::VisitorBSON<>,
    rpp::VisitorBSONView<>
>;

// standard ajax

template <class Args>
void ajaxArgs(cgicc::FCgiCC<> &cgi, Args &args) {
    RPP_META_DYNAMIC(
        "args", Args, VisitorListArgs
    ) meta{args};

    rpp::VisitorIStrTree<cgicc::FCgiCC<>> visitor{cgi};
    meta.doVisit(visitor);
}

template <class Result>
void ajaxReturn(cgicc::FCgiCC<> &cgi, Result &result) {
    cgi << content_type_json;

    RPP_META_DYNAMIC(
        "result", Result, VisitorListJSON
    ) meta{result};

    rpp::VisitorJSON<cgicc::FCgiCC<>> visitor{cgi};
    meta.doVisit(visitor);
};

// helper macro and functions

#define BOOKSTORE_DB_CONN() \
    mongocxx::instance inst{}; \
    mongocxx::client conn{}; \
    auto db = conn["bookstore"]; \
    auto db_cat = db["cat"]; \
    auto db_book = db["book"]; \
    auto db_user = db["user"]; \
    auto db_buy = db["buy"];

#define BOOKSTORE_MAIN(Exec, Err) \
    int main() { \
        using namespace bookstore; \
        fcgicc_exec((Exec), (Err)); \
        return 0; \
    }

inline std::string genOID() {
    return bsoncxx::oid{bsoncxx::oid::init_tag}.to_string();
}

// TODO: not implemented
inline std::string passwordHash(const std::string user, const std::string pass) {
    return user + pass; // TODO
}

}
