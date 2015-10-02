#pragma once

#include "reflection++/visitor/strtree.hpp"
#include "reflection++/visitor/json.hpp"
#include "reflection++/visitor/bson.hpp"
#include "reflection++/visitor/bson_view.hpp"
#include "reflection++/meta.hpp"

namespace wpp {

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

template <class T>
void bsonGet(const bsoncxx::document::view &view, T &value) {
    RPP_META_DYNAMIC(
        "data", T, VisitorListDB
    ) meta{value};

    rpp::VisitorBSONView<> visitor{
        bsoncxx::types::b_document{view}
    };
    meta.doVisit(visitor);
}

template <class T>
rpp::VisitorBSON<> bsonGen(T &&value) {
    RPP_META_DYNAMIC(
        "data", T, VisitorListDB
    ) meta{value};

    rpp::VisitorBSON<> visitor{};
    meta.doVisit(visitor);

    return std::move(visitor);
}

}
