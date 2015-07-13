#include "bookstore_headers.hpp"

namespace wpp {

BOOKSTORE_DB_CONN()

struct Args {
    std::string book_id;
    std::string address;
};

RPP_TYPE_OBJECT(
    __(book_id)
    __(address),
    Args
)

struct Result {
    bool set_success;
};

RPP_TYPE_OBJECT(
    __(set_success),
    Result
)

void exec(cgicc::FCgiCC<> &cgi) {
    static_cast<void>(cgi); // TODO
}

}

BOOKSTORE_MAIN(exec, ignoreErr)
