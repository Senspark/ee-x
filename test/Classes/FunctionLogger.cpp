//
//  FunctionLogger.cpp
//  ee_x_test
//
//  Created by Zinge on 10/30/17.
//
//

#include "FunctionLogger.hpp"
#include "Utils.hpp"

namespace eetest {
using Self = FunctionLogger;

Self::FunctionLogger(const std::string& function) {
    function_ = function;
    getLogger().info("%s: begin", function_.c_str());
}

Self::~FunctionLogger() {
    getLogger().info("%s: end", function_.c_str());
}
} // namespace eetest
