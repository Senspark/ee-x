#ifndef EE_X_I_FIREBASE_CRASHLYTICS_BRIDGE_HPP
#define EE_X_I_FIREBASE_CRASHLYTICS_BRIDGE_HPP

#ifdef __cplusplus

#include <string>

#include <ee/core/IPlugin.hpp>

#include "ee/firebase_crashlytics/FirebaseCrashlyticsFwd.hpp"

namespace ee {
namespace firebase {
namespace crashlytics {
class IBridge : public IPlugin {
public:
    [[nodiscard]] virtual Task<bool> initialize() = 0;
    virtual void log(const std::string& message) = 0;
};
} // namespace crashlytics
} // namespace firebase
} // namespace ee

#endif // __cplusplus

#endif // EE_X_I_FIREBASE_CRASHLYTICS_BRIDGE_HPP
