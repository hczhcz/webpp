#pragma once

#include "reflection++/visitor_chain.hpp"
#include "reflection++/visitor/bson.hpp"
#include "reflection++/visitor/bson_view.hpp"

namespace bookstore {

RPP_VISITOR_CHAIN_INIT()

// RPP_VISITOR_REG(rpp::VisitorIStrTree<cgicc::FCgiCC<>>)
// RPP_VISITOR_REG(rpp::VisitorJSON<cgicc::FCgiCC<>>)
RPP_VISITOR_REG(rpp::VisitorBSON<>)
RPP_VISITOR_REG(rpp::VisitorBSONView<>)

RPP_VISITOR_COLLECT(VisitorList)

}
