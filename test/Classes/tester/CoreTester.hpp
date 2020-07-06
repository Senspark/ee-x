//
//  CoreTester.hpp
//  ee_x_test
//
//  Created by eps on 7/6/20.
//

#ifndef EE_X_TEST_CORE_TESTER_HPP
#define EE_X_TEST_CORE_TESTER_HPP

#include "ITester.hpp"

namespace eetest {
namespace core {
class Tester : public ITester {
public:
    virtual void initialize() override;
    virtual void destroy() override;
    virtual void start() override;
    virtual void stop() override;

private:
};
} // namespace core

using CoreTester = core::Tester;
} // namespace eetest

#endif /* EE_X_TEST_CORE_TESTER_HPP */
