#include "bookstore_headers.hpp"

namespace wpp {

BOOKSTORE_DB_CONN()

struct Args {
    std::string name;
    std::string password;
};

RPP_TYPE_OBJECT(
    __(name) __(password),
    Args
)

struct Result {
    bool auth_success;
    Maybe<std::string> auth_user_id;
    Maybe<std::string> auth_name;
    bool auth_sudo;
};

RPP_TYPE_OBJECT(
    __(auth_success) __(auth_user_id) __(auth_name) __(auth_sudo),
    Result
)

void exec(cgicc::FCgiCC<> &cgi) {
    BOOKSTORE_EXEC_ENTER(session, args)

    std::string salt{session.auth_salt.force()};

    Result result;

    using namespace bsoncxx::builder::stream;

    User user;
    if (dbGetOneFiltered(
        db_user, user,
        document{} << "name" << args.name << finalize,
        false
    )) {
        if (hashStr(user.password + salt) == args.password) {
            // login ok

            session.auth_salt = nullptr;
            session.auth_user_id = user._id;
            session.auth_name = user.name;
            session.auth_sudo = user.is_admin;

            result = Result{
                true,
                session.auth_user_id.force(),
                session.auth_name.force(),
                session.auth_sudo
            };
        } else {
            // wrong password

            result = Result{false, user.name, nullptr, false};
        }
    } else {
        // wrong name

        result = Result{false, nullptr, nullptr, false};
    }

    BOOKSTORE_EXEC_EXIT(result, session)
}

}

BOOKSTORE_MAIN(exec, ignoreErr)
