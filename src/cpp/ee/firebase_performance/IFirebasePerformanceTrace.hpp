#ifndef EE_X_I_FIREBASE_PERFORMANCE_TRACE_HPP
#define EE_X_I_FIREBASE_PERFORMANCE_TRACE_HPP

#ifdef __cplusplus

#include <cstdint>
#include <string>

#include "ee/firebase_performance/FirebasePerformanceFwd.hpp"

namespace ee {
namespace firebase {
namespace performance {
class ITrace {
public:
    virtual ~ITrace() = default;

    virtual void start() = 0;
    virtual void stop() = 0;

    virtual void putMetric(const std::string& metricName,
                           std::int64_t value) = 0;
    virtual void incrementMetric(const std::string& metricName,
                                 std::int64_t incrementBy) = 0;
    virtual std::int64_t getLongMetric(const std::string& metricName) = 0;
};
} // namespace performance
} // namespace firebase
} // namespace ee

#endif // __cplusplus

#endif // EE_X_I_FIREBASE_PERFORMANCE_TRACE_HPP