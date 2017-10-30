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
    getLogger().info(function_, ": begin");
}

Self::~FunctionLogger() {
    getLogger().info(function_, ": end");
}
} // namespace eetest
