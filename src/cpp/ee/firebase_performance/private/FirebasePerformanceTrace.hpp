//
//  FirebasePerformanceTrace.hpp
//  ee-x
//
//  Created by Nguyen Dinh Phuoc Duc on 3/8/19.
//

#ifndef EE_X_FIREBASE_PERFORMANCE_TRACE_HPP
#define EE_X_FIREBASE_PERFORMANCE_TRACE_HPP

#include "ee/firebase_performance/IFirebasePerformanceTrace.hpp"

namespace ee {
namespace firebase {
namespace performance {
class Trace : public ITrace {
public:
    explicit Trace(IMessageBridge& bridge, Bridge* plugin,
                   const std::string& name);

    virtual void start() override;
    virtual void stop() override;

    virtual void putMetric(const std::string& metricName,
                           std::int64_t value) override;
    virtual void incrementMetric(const std::string& metricName,
                                 std::int64_t incrementBy) override;
    virtual std::int64_t getLongMetric(const std::string& metricName) override;

private:
    IMessageBridge& bridge_;
    Bridge* plugin_;
    std::string name_;
};
} // namespace performance
} // namespace firebase
} // namespace ee

#endif /* EE_X_FIREBASE_PERFORMANCE_TRACE_HPP */
