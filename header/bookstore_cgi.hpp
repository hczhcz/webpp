#pragma once

#include "reflection++/visitor_chain.hpp"
#include "reflection++/visitor/strtree.hpp"
#include "reflection++/visitor/json.hpp"

#include "cgiutil.hpp"

namespace cgicc {

RPP_VISITOR_CHAIN_INIT()
RPP_VISITOR_REG(rpp::VisitorIStrTree<FCgiCC<>>)
RPP_VISITOR_REG(rpp::VisitorJSON<FCgiCC<>>)
RPP_VISITOR_COLLECT(VisitorList)

}
