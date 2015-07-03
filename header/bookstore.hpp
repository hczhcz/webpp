// pch support
#ifndef BOOKSTORE_HPP
#define BOOKSTORE_HPP

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
#include "bookstore_hash.hpp"
#include "bookstore_cgi.hpp"

namespace bookstore {

// helper macros

#define BOOKSTORE_DB_CONN() \
    mongocxx::instance inst{}; \
    mongocxx::client conn{}; \
    auto db = conn["bookstore"]; \
    auto db_cat = db["cat"]; \
    auto db_book = db["book"]; \
    auto db_user = db["user"]; \
    auto db_buy = db["buy"]; \
    auto db_session = db["session"];

#define BOOKSTORE_MAIN(Exec, Err) \
    int main() { \
        using namespace bookstore; \
        fcgicc_exec((Exec), (Err)); \
        return 0; \
    }

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

template <class Session, class Result>
void ajaxReturn(cgicc::FCgiCC<> &cgi, Session &session, Result &result) {
    cgicc::HTTPContentHeader header{content_type_json};
    header.setCookie(cgicc::HTTPCookie{session_tag_id, session._id});
    header.setCookie(cgicc::HTTPCookie{session_tag_id, session.key});
    cgi << header;

    RPP_META_DYNAMIC(
        "result", Result, VisitorListJSON
    ) meta{result};

    rpp::VisitorJSON<cgicc::FCgiCC<>> visitor{cgi};
    meta.doVisit(visitor);
};

// standard database io

template <class T>
void dbGet(const bsoncxx::document::view &view, T &value) {
    RPP_META_DYNAMIC(
        "data", T, VisitorListDB
    ) meta{value};

    rpp::VisitorBSONView<> visitor{
        bsoncxx::types::b_document{view}
    };
    meta.doVisit(visitor);
}

template <class T>
std::string dbInsert(mongocxx::collection &db, T &&value) {
    RPP_META_DYNAMIC(
        "data", T, VisitorListDB
    ) meta{value};

    rpp::VisitorBSON<> visitor{};
    meta.doVisit(visitor);

    db.insert_one(visitor.view());

    return value._id;
}

// session

template <class Session = Session>
Session makeSession(
        cgicc::FCgiCC<> &cgi,
        mongocxx::collection &db
) {
    std::string id;
    std::string key;

    for (const auto &item: cgi.getEnvironment().getCookieList()) {
        std::string name = item.getName();
        if (name == session_tag_id) {
            id = item.getValue();
        } else if (name == session_tag_key) {
            key = item.getValue();
        }
    }

    // find session in the database
    if (id != "") {
        using namespace bsoncxx::builder::stream;
        Session session;

        auto cursor = db.find(
            document{}
                << "_id" << key << finalize
        );

        if (cursor.begin() != cursor.end()) {
            dbGet(*cursor.begin(), session);

            // check key and time
            if (session.key == key) {
                if (
                    difftime(
                        time(nullptr), session.date_create
                    ) <= 60 * 60 * 24
                ) {
                    return session;
                }
            }
        }
    }

    // new session
    return Session{
        genOID(), randstr(),
        nullptr, nullptr, false,
        time(nullptr)
    };
}

}

#endif
