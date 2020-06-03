//
//  FirebasePerformance.hpp
//  ee-x
//
//  Created by Nguyen Dinh Phuoc Duc on 3/8/19.
//

#ifndef EE_X_FIREBASE_PERFORMANCE_BRIDGE_HPP
#define EE_X_FIREBASE_PERFORMANCE_BRIDGE_HPP

#ifdef __cplusplus

#include <memory>
#include <string>

#include <ee/FirebaseFwd.hpp>

/// Wrapper for Firebase Performance.
/// https://firebase.google.com/docs/perf-mon/get-started-ios
/// https://firebase.google.com/docs/perf-mon/get-started-android
/// https://firebase.google.com/docs/reference/ios/firebaseperformance/api/reference/Classes/FIRPerformance

namespace ee {
namespace firebase {
namespace performance {
class Bridge final {
public:
    Bridge() = default;
    ~Bridge() = default;

    /// @property dataCollectionEnabled
    /// @see
    /// https://firebase.google.com/docs/reference/ios/firebaseperformance/api/reference/Classes/FirebasePerformance#/c:objc(cs)FIRPerformance(py)dataCollectionEnabled
    void setDataCollectionEnabled(bool enabled);
    bool isDataCollectionEnabled();

    /// Creates an instance of FIRTrace after creating the shared instance of
    /// FirebasePerformance. The trace will automatically be started on a
    /// successful creation of the instance. The |name| of the trace cannot be
    /// an empty string.
    std::shared_ptr<Trace> startTrace(const std::string& name);

    /// Creates an instance of FIRTrace. This API does not start the trace. To
    /// start the trace, use the -start API on the returned |FIRTrace| object.
    /// The |name| cannot be an empty string.
    std::shared_ptr<Trace> newTrace(const std::string& name);
};
} // namespace performance
} // namespace firebase
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_FIREBASE_PERFORMANCE_BRIDGE_HPP */
