//
//  FirebasePerformanceTrace.hpp
//  ee-x
//
//  Created by Nguyen Dinh Phuoc Duc on 3/8/19.
//

#ifndef EE_X_FIREBASE_PERFORMANCE_TRACE_HPP_
#define EE_X_FIREBASE_PERFORMANCE_TRACE_HPP_

#include "ee/firebase/FirebasePerformanceBridge.hpp"

namespace ee {
namespace firebase {
class Performance;

class PerformanceTrace {
public:
    ~PerformanceTrace() = default;

    void start();

    void stop();

    void putMetric(const std::string& metricName, std::int64_t value);
    void incrementMetric(const std::string& metricName,
                         std::int64_t incrementBy);
    std::int64_t getLongMetric(const std::string& metricName);
    
protected:
    friend Performance;
    
private:
    explicit PerformanceTrace(Performance* plugin, const std::string& name);
    
private:
    Performance* plugin_;
    std::string name_;
};
} // namespace firebase
} // namespace ee

#endif /* EE_X_FIREBASE_PERFORMANCE_TRACE_HPP_ */
