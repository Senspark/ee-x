#ifndef EE_X_I_FIREBASE_PERFORMANCE_BRIDGE_HPP
#define EE_X_I_FIREBASE_PERFORMANCE_BRIDGE_HPP

#ifdef __cplusplus

#include <memory>
#include <string>

#include <ee/core/IPlugin.hpp>

#include "ee/firebase_performance/FirebasePerformanceFwd.hpp"

namespace ee {
namespace firebase {
namespace performance {
/// Wrapper for Firebase Performance.
/// https://firebase.google.com/docs/perf-mon/get-started-ios
/// https://firebase.google.com/docs/perf-mon/get-started-android
/// https://firebase.google.com/docs/reference/ios/firebaseperformance/api/reference/Classes/FIRPerformance
class IBridge : public IPlugin {
public:
    /// @property dataCollectionEnabled
    /// @see
    /// https://firebase.google.com/docs/reference/ios/firebaseperformance/api/reference/Classes/FirebasePerformance#/c:objc(cs)FIRPerformance(py)dataCollectionEnabled
    virtual bool isDataCollectionEnabled() = 0;
    virtual void setDataCollectionEnabled(bool enabled) = 0;

    /// Creates an instance of FIRTrace. This API does not start the trace. To
    /// start the trace, use the -start API on the returned |FIRTrace| object.
    /// The |name| cannot be an empty string.
    virtual std::shared_ptr<ITrace> newTrace(const std::string& name) = 0;
};
} // namespace performance
} // namespace firebase
} // namespace ee

#endif // __cplusplus

#endif // EE_X_I_FIREBASE_PERFORMANCE_BRIDGE_HPP