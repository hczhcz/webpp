#pragma once

#include "reflection++/accessor_infer.hpp"

#include <memory>
#include <vector>

namespace wpp {

// helper macros

#define BOOKSTORE_DB_CONN() \
    mongocxx::instance inst{}; \
    mongocxx::client conn{}; \
    \
    auto db = conn["bookstore"]; \
    auto db_cat = db["cat"]; \
    auto db_book = db["book"]; \
    auto db_user = db["user"]; \
    auto db_buy = db["buy"]; \
    auto db_session = db["session"];

#define BOOKSTORE_MAIN(Exec, Err) \
    int main() { \
        using namespace wpp; \
        fcgiccExec((Exec), (Err)); \
        \
        return 0; \
    }

#define BOOKSTORE_EXEC_ENTER(SObj, AObj) \
    Session (SObj); \
    makeSession(cgi, db_session, (SObj)); \
    Args (AObj); \
    ajaxArgs(cgi, (AObj));

#define BOOKSTORE_EXEC_EXIT(RObj, SObj) \
    ajaxReturn(cgi, (SObj), (RObj)); \
    saveSession(db_session, (SObj)); \
    return;

// rpp setup

RPP_ACCESSOR_INFER_INIT()

// template <class T, size_t size>
// RPP_TYPE_DYNAMIC_GENERIC(T, T [size])
template <class T, class... Args>
RPP_TYPE_DYNAMIC_GENERIC(T, std::vector<T, Args...>)
// template <class Key, class T, class... Args>
// RPP_TYPE_DYNAMIC_GENERIC(T, std::unordered_map<Key, T, Args...>)
// template <class T, class... Args>
// RPP_TYPE_DYNAMIC_GENERIC(T, std::unique_ptr<T, Args...>)

template <class T>
struct Maybe: public std::unique_ptr<T> {
    Maybe(): std::unique_ptr<T>{} {}

    Maybe(std::nullptr_t): std::unique_ptr<T>{} {}

    template <class T1>
    Maybe(T1 &&value): std::unique_ptr<T>{
        new T{std::move(value)}
    } {}

    template <class T1>
    Maybe(const T1 &value): std::unique_ptr<T>{
        new T{value}
    } {}

    Maybe<T> &operator=(std::nullptr_t) {
        std::unique_ptr<T>::operator=(nullptr);

        return *this;
    }

    template <class T1>
    Maybe<T> &operator=(const T1 &value) {
        std::unique_ptr<T>::operator=(
            std::unique_ptr<T>{new T{value}}
        );

        return *this;
    }

    T &force() {
        if (std::unique_ptr<T>::get()) {
            return *std::unique_ptr<T>::get();
        } else {
            throw std::exception{};
        }
    }
};

template <class T>
RPP_TYPE_DYNAMIC_GENERIC(T, Maybe<T>)

template <class T>
struct Subset: public T {};

template <class T>
struct Subset2: public T {};

struct User {
    std::string _id; // user_id

    std::string mail;
    std::string name;
    Maybe<std::string> image;
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

RPP_TYPE_OBJECT(
    __(_id)
    __(name) __(image) __(detail) __(location)
    __(book_count) __(sold_count),
    Subset<User>
)

RPP_TYPE_OBJECT(
    __(_id)
    __(name)
    __(book_count) __(sold_count),
    Subset2<User>
)

struct Cat {
    std::string _id; // cat_id
    std::string parent_cat_id;

    std::string name;
    Maybe<std::string> image;
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

RPP_TYPE_OBJECT(
    __(_id)
    __(name) __(detail)
    __(tot_book_count),
    Subset<Cat>
)

struct Book {
    std::string _id; // book_id
    std::string owner_user_id;
    std::string parent_cat_id;

    std::string name;
    Maybe<std::string> image;
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

RPP_TYPE_OBJECT(
    __(_id)
    __(name)
    __(sold_count),
    Subset<Book>
)

RPP_TYPE_OBJECT(
    __(_id)
    __(name) __(detail) __(price) __(inventory)
    __(sold_count),
    Subset2<Book>
)

struct Buy {
    std::string _id; // buy_id
    std::string buyer_user_id;
    std::string buy_book_id; // seller_user_id, book_name from buy_book_id

    std::string address;
    Maybe<std::string> feedback;

    time_t date_create;
    Maybe<time_t> date_accept;
    Maybe<time_t> date_done;
};

RPP_TYPE_OBJECT(
    __(_id) __(buyer_user_id) __(buy_book_id)
    __(address) __(feedback)
    __(date_create) __(date_accept) __(date_done),
    Buy
)

RPP_TYPE_OBJECT(
    __(_id) __(buy_book_id)
    __(address)
    __(date_accept) __(date_done),
    Subset<Buy>
)

RPP_TYPE_OBJECT(
    __(feedback)
    __(date_done),
    Subset2<Buy>
)

struct Session {
    std::string _id;
    std::string key;

    Maybe<std::string> auth_salt;
    Maybe<std::string> auth_user_id;
    Maybe<std::string> auth_name;
    bool auth_sudo; // TODO: currently not in use

    time_t date_create;

    Session() {} // uninitialized struct

    Session(const std::string &to_id, const std::string &to_key):
        _id{to_id}, key{to_key}, date_create{time(nullptr)} {}
};

RPP_TYPE_OBJECT(
    __(_id) __(key)
    __(auth_salt) __(auth_user_id) __(auth_name) __(auth_sudo)
    __(date_create),
    Session
)

}
