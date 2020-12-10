//
//  FirebaseCrashlyticsTester.hpp
//  ee_x_test
//
//  Created by eps on 7/6/20.
//

#ifndef EE_X_TEST_FIREBASE_CRASHLYTICS_TESTER_HPP
#define EE_X_TEST_FIREBASE_CRASHLYTICS_TESTER_HPP

#include <memory>

#include <ee/FirebaseFwd.hpp>

#include "ITester.hpp"

namespace eetest {
namespace firebase {
namespace crashlytics {
class Tester : public ITester {
public:
    virtual void initialize() override;
    virtual void destroy() override;
    virtual void start() override;
    virtual void stop() override;

private:
    std::shared_ptr<ee::FirebaseCrashlytics> plugin_;
};
} // namespace crashlytics
} // namespace firebase

using FirebaseCrashlyticsTester = firebase::crashlytics::Tester;
} // namespace eetest

#endif /* EE_X_TEST_FIREBASE_CRASHLYTICS_TESTER_HPP */
