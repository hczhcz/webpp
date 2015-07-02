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

template <class DB, class T>
std::string db_insert(DB &db, T value) {
    rpp::VisitorBSON<> doc{};

    RPP_META_DYNAMIC(
        "data", T, VisitorList
    ) meta{value};

    meta.doVisit(doc);

    db.insert_one(doc.view());

    return value._id.to_string();
}

void exec(cgicc::FCgiCC<> &cgi) {
    mongocxx::instance inst{};
    mongocxx::client conn{};

    auto db = conn["bookstore"];
    auto db_cat = db["cat"];
    auto db_book = db["book"];
    auto db_user = db["user"];
    auto db_buy = db["buy"];

    db_cat.delete_many({});
    db_book.delete_many({});
    db_user.delete_many({});
    db_buy.delete_many({});

    std::string cat_id_st = db_insert(db_cat, Cat{
        oid_str{}, maybe<std::string>{},
        "科学技术", "13dd167159a652a278fe0860a0abf468", "这是科学技术类图书。",
        0, 0, 0
    });

    db_insert(db_cat, Cat{
        oid_str{}, maybe<std::string>{},
        "休闲娱乐", "1155b9cf0f24f8d2c2da854e62a4bbef", "这是休闲娱乐类图书。",
        0, 0, 0
    });

    db_insert(db_cat, Cat{
        oid_str{}, maybe<std::string>{},
        "医疗健康", "59d9e827ccb1947002ac8acd50e38e17", "这是医疗健康类图书。",
        0, 0, 0
    });

    db_insert(db_cat, Cat{
        oid_str{}, maybe<std::string>{},
        "外文图书", "720b75954179401f4f80bc8a91bc90e7", "Books in English are listed here.",
        0, 0, 0
    });

    db_insert(db_cat, Cat{
        oid_str{}, maybe<std::string>{cat_id_st},
        "物理", "2f861d02192743a1c1cf2f32adec97b8", "这是科学技术类中的物理类图书。",
        0, 0, 0
    });

    db_insert(db_cat, Cat{
        oid_str{}, maybe<std::string>{cat_id_st},
        "化学", "732e1de1c5a204f76d869059462e8481", "这是科学技术类中的化学类图书。",
        0, 0, 0
    });

    db_insert(db_cat, Cat{
        oid_str{}, maybe<std::string>{cat_id_st},
        "计算机", "ae2e4dbc9b2a993cd9ee98f0a37e8319", "这是科学技术类中的计算机类图书。",
        0, 0, 0
    });

    cgi << "Content-Type: text/plain; charset=utf-8;\r\n\r\n";
    cgi << "ok\r\n";
}

void err(const std::exception &) {
    // ignore
}

}

int main() {
    fcgicc_exec(bookstore::exec, bookstore::err);

    return 0;
}
