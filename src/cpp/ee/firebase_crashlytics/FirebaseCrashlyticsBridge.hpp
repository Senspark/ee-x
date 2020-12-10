#ifndef EE_X_FIREBASE_CRASHLYTICS_BRIDGE_HPP
#define EE_X_FIREBASE_CRASHLYTICS_BRIDGE_HPP

#ifdef __OBJC__
#import <FirebaseCrashlytics/FIRCrashlytics.h>
#endif // __OBJC__

#ifdef __cplusplus

#include <string>

#include <ee/FirebaseFwd.hpp>
#include <ee/core/IPlugin.hpp>

namespace ee {
namespace firebase {
namespace crashlytics {
class Bridge final : public IPlugin {
public:
    Bridge();
    ~Bridge();

    virtual void destroy() override;

    void log(const std::string& message);

private:
    IMessageBridge& bridge_;
};
} // namespace crashlytics
} // namespace firebase
} // namespace ee

#endif // __cplusplus

#endif // EE_X_FIREBASE_CRASHLYTICS_BRIDGE_HPP
