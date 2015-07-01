#pragma once

#include "reflection++/accessor_infer.hpp"

namespace bookstore {

RPP_ACCESSOR_INFER_INIT()

struct oid_str: public bsoncxx::oid {
    oid_str(): bsoncxx::oid{bsoncxx::oid::init_tag} {}
    oid_str(const std::string &str): bsoncxx::oid{str} {}

    operator std::string() const {
        return to_string();
    }

    oid_str &operator=(const std::string &str) {
        this->~oid_str();
        new(this) oid_str{str};

        return *this;
    }
};

struct User {
    oid_str _id; // user_id

    std::string mail;
    std::string name;
    std::string image;
    std::string detail;
    std::string password;
    bool is_admin;
    std::string location;
    std::string address;

    long bought_count;
    long book_count;
    long sold_count;

    std::time_t date_create;
    std::time_t date_login;
};

struct Cat {
    oid_str _id; // cat_id
    oid_str parent_cat_id;

    std::string name;
    std::string image;
    std::string detail;

    long cat_count;
    long tot_book_count;
    long book_count;
};

struct Book {
    oid_str _id; // book_id
    oid_str owner_user_id;
    oid_str parent_cat_id;

    std::string name;
    std::string image;
    std::string detail;
    std::string isbn; // new
    std::string price;
    long inventory;

    long sold_count;

    std::time_t date_create;
};

struct Buy {
    oid_str _id; // buy_id
    oid_str buyer_user_id;
    oid_str buy_book_id; // seller_user_id, book_name from buy_book_id

    std::string address;
    std::string feedback;

    std::time_t date_create;
    std::time_t date_accept;
    std::time_t date_done;
};

}
