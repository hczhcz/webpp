#include "../header/bookstore.hpp"

namespace bookstore {

BOOKSTORE_DB_CONN()

struct Args {
    std::string buy_id;
};

RPP_TYPE_OBJECT(
    __(buy_id),
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
