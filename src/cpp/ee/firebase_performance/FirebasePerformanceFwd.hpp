#ifndef EE_X_FIREBASE_PERFORMANCE_FWD_HPP
#define EE_X_FIREBASE_PERFORMANCE_FWD_HPP

#ifdef __cplusplus

#include <ee/core/CoreFwd.hpp>

namespace ee {
namespace firebase {
namespace performance {
class IBridge;
class ITrace;
class Bridge;
class Trace;
} // namespace performance
} // namespace firebase

using IFirebasePerformance = firebase::performance::IBridge;
using IFirebasePerformanceTrace = firebase::performance::ITrace;
} // namespace ee

#endif // __cplusplus

#endif // EE_X_FIREBASE_PERFORMANCE_FWD_HPP