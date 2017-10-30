//
//  FunctionLogger.hpp
//  ee_x_test
//
//  Created by Zinge on 10/30/17.
//
//

#ifndef EE_X_TEST_FUNCTION_LOGGER_HPP
#define EE_X_TEST_FUNCTION_LOGGER_HPP

#include <string>

namespace eetest {
class FunctionLogger {
public:
    FunctionLogger(const std::string& function);
    ~FunctionLogger();

private:
    std::string function_;
};
} // namespace eetest

#endif /* EE_X_TEST_FUNCTION_LOGGER_HPP */
