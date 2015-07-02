#pragma once

#include <memory>

#include <bsoncxx/oid.hpp>

#include "reflection++/accessor_infer.hpp"

namespace bookstore {

RPP_ACCESSOR_INFER_INIT()

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

template <class T>
struct maybe: public std::unique_ptr<T> {
    maybe(): std::unique_ptr<T>{} {}

    maybe(T &&value): std::unique_ptr<T>{
        new T{std::move(value)}
    } {}

    maybe(const T &value): std::unique_ptr<T>{
        new T{value}
    } {}
};

template <class T>
RPP_TYPE_DYNAMIC_GENERIC(T, maybe<T>)

struct Cat {
    oid_str _id; // cat_id
    maybe<std::string> parent_cat_id;

    std::string name;
    std::string image;
    std::string detail;

    long cat_count;
    long tot_book_count;
    long book_count;
};

RPP_TYPE_OBJECT(
    __(_id) __(parent_cat_id)
    __(name) __(image) __(detail)
    __(cat_count) __(tot_book_count) __(book_count),
    Cat
)

struct Book {
    oid_str _id; // book_id
    std::string owner_user_id;
    std::string parent_cat_id;

    std::string name;
    std::string image;
    std::string detail;
    std::string isbn; // new
    std::string price;
    long inventory;

    long sold_count;

    std::time_t date_create;
};

RPP_TYPE_OBJECT(
    __(_id) __(owner_user_id) __(parent_cat_id)
    __(name) __(image) __(detail) __(isbn) __(price) __(inventory)
    __(sold_count)
    __(date_create),
    Book
)

struct User {
    oid_str _id; // user_id

    std::string mail;
    std::string name;
    std::string image;
    std::string detail;
    std::string password;
    std::string location;
    std::string address;

    bool is_admin;
    long bought_count;
    long book_count;
    long sold_count;

    std::time_t date_create;
    std::time_t date_login;
};

RPP_TYPE_OBJECT(
    __(_id)
    __(mail) __(name) __(image) __(detail) __(password) __(location) __(address)
    __(is_admin) __(bought_count) __(book_count) __(sold_count)
    __(date_create) __(date_login),
    User
)

struct Buy {
    oid_str _id; // buy_id
    std::string buyer_user_id;
    std::string buy_book_id; // seller_user_id, book_name from buy_book_id

    std::string address;
    maybe<std::string> feedback;

    std::time_t date_create;
    maybe<std::time_t> date_accept;
    maybe<std::time_t> date_done;
};

RPP_TYPE_OBJECT(
    __(_id) __(buyer_user_id) __(buy_book_id)
    __(address) __(feedback)
    __(date_create) __(date_accept) __(date_done),
    Buy
)

}
