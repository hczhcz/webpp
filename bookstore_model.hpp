#pragma once

#include <memory>

#include "reflection++/accessor_infer.hpp"

namespace bookstore {

RPP_ACCESSOR_INFER_INIT()

template <class T>
struct maybe: public std::unique_ptr<T> {
    maybe(): std::unique_ptr<T>{} {}

    maybe(std::nullptr_t): std::unique_ptr<T>{} {}

    template <class T1>
    maybe(T1 &&value): std::unique_ptr<T>{
        new T{std::move(value)}
    } {}

    template <class T1>
    maybe(const T1 &value): std::unique_ptr<T>{
        new T{value}
    } {}
};

template <class T>
RPP_TYPE_DYNAMIC_GENERIC(T, maybe<T>)

struct User {
    std::string _id; // user_id

    std::string mail;
    std::string name;
    maybe<std::string> image;
    std::string detail;
    std::string password;
    std::string location;
    std::string address;

    bool is_admin;
    long bought_count;
    long book_count;
    long sold_count;

    time_t date_create;
    time_t date_login;
};

RPP_TYPE_OBJECT(
    __(_id)
    __(mail) __(name) __(image) __(detail) __(password) __(location) __(address)
    __(is_admin) __(bought_count) __(book_count) __(sold_count)
    __(date_create) __(date_login),
    User
)

struct Cat {
    std::string _id; // cat_id
    maybe<std::string> parent_cat_id;

    std::string name;
    maybe<std::string> image;
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
    std::string _id; // book_id
    std::string owner_user_id;
    std::string parent_cat_id;

    std::string name;
    maybe<std::string> image;
    std::string detail;
    std::string isbn; // new
    std::string price;
    long inventory;

    long sold_count;

    time_t date_create;
};

RPP_TYPE_OBJECT(
    __(_id) __(owner_user_id) __(parent_cat_id)
    __(name) __(image) __(detail) __(isbn) __(price) __(inventory)
    __(sold_count)
    __(date_create),
    Book
)

struct Buy {
    std::string _id; // buy_id
    std::string buyer_user_id;
    std::string buy_book_id; // seller_user_id, book_name from buy_book_id

    std::string address;
    maybe<std::string> feedback;

    time_t date_create;
    maybe<time_t> date_accept;
    maybe<time_t> date_done;
};

RPP_TYPE_OBJECT(
    __(_id) __(buyer_user_id) __(buy_book_id)
    __(address) __(feedback)
    __(date_create) __(date_accept) __(date_done),
    Buy
)

}
