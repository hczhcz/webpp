#pragma once

#include <bsoncxx/oid.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include "reflection++/visitor_chain.hpp"
#include "reflection++/visitor/strtree.hpp"
#include "reflection++/visitor/json.hpp"
#include "reflection++/visitor/bson.hpp"
#include "reflection++/visitor/bson_view.hpp"
#include "reflection++/meta.hpp"

#include "cgiutil.hpp"

namespace bookstore {

// TODO: simplify this
struct oid_str: public bsoncxx::oid {
    oid_str(): bsoncxx::oid{
        bsoncxx::oid::init_tag
    } {}

    oid_str(const std::string &str): bsoncxx::oid{
        bsoncxx::stdx::string_view{str}
    } {}

    operator std::string() const {
        return to_string();
    }

    oid_str &operator=(const std::string &str) {
        this->~oid_str();
        new(this) oid_str{str};

        return *this;
    }
};

RPP_VISITOR_CHAIN_INIT()

// RPP_VISITOR_REG(rpp::VisitorIStrTree<cgicc::FCgiCC<>>)
// RPP_VISITOR_REG(rpp::VisitorJSON<cgicc::FCgiCC<>>)
RPP_VISITOR_REG(rpp::VisitorBSON<>)
RPP_VISITOR_REG(rpp::VisitorBSONView<>)

RPP_VISITOR_COLLECT(VisitorList)

}
