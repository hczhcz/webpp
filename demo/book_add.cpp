#include "bookstore_headers.hpp"

namespace wpp {

BOOKSTORE_DB_CONN()

struct Args {
    //
};

RPP_TYPE_OBJECT(
    // __(???),
    ,
    Args
)

void exec(cgicc::FCgiCC<> &cgi) {
    static_cast<void>(cgi); // TODO
}

}

BOOKSTORE_MAIN(exec, ignoreErr)
