//
//  ITester.hpp
//  ee_x_test
//
//  Created by eps on 7/6/20.
//

#ifndef EE_X_TEST_I_TESTER_HPP
#define EE_X_TEST_I_TESTER_HPP

#include "AppMacro.hpp"

namespace eetest {
class ITester {
public:
    virtual ~ITester() = default;
    virtual void initialize() = 0;
    virtual void destroy() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
};
} // namespace eetest

#endif /* EE_X_TEST_I_TESTER_HPP */
