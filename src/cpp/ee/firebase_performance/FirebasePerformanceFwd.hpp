#ifndef EE_X_FIREBASE_PERFORMANCE_FWD_HPP
#define EE_X_FIREBASE_PERFORMANCE_FWD_HPP

#ifdef __cplusplus

#include <ee/core/CoreFwd.hpp>

namespace ee {
namespace firebase {
namespace performance {
class Bridge;
class Trace;
} // namespace performance
} // namespace firebase

using FirebasePerformance = firebase::performance::Bridge;
using FirebasePerformanceTrace = firebase::performance::Trace;
} // namespace ee

#endif // __cplusplus

#endif // EE_X_FIREBASE_PERFORMANCE_FWD_HPP