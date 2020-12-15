#ifndef EE_X_FIREBASE_CRASHLYTICS_FWD_HPP
#define EE_X_FIREBASE_CRASHLYTICS_FWD_HPP

#ifdef __cplusplus

#include <ee/core/CoreFwd.hpp>

namespace ee {
namespace firebase {
namespace crashlytics {
class IBridge;
class Bridge;
} // namespace crashlytics
} // namespace firebase

using IFirebaseCrashlytics = firebase::crashlytics::IBridge;
} // namespace ee

#endif // __cplusplus

#endif // EE_X_FIREBASE_CRASHLYTICS_FWD_HPP