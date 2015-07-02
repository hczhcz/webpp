#pragma once

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/find.hpp>
#include <bsoncxx/oid.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include "reflection++/meta.hpp"

#include "bookstore_model.hpp"
#include "bookstore_bson.hpp"
#include "bookstore_cgi.hpp"

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

}
