//
//  FirebasePerformanceTrace.hpp
//  ee-x
//
//  Created by Nguyen Dinh Phuoc Duc on 3/8/19.
//

#ifndef EE_X_FIREBASE_PERFORMANCE_TRACE_HPP
#define EE_X_FIREBASE_PERFORMANCE_TRACE_HPP

#include <cstdint>
#include <string>

#include <ee/FirebaseFwd.hpp>

namespace ee {
namespace firebase {
namespace performance {
class Trace {
public:
    ~Trace() = default;

    void start();

    void stop();

    void putMetric(const std::string& metricName, std::int64_t value);
    void incrementMetric(const std::string& metricName,
                         std::int64_t incrementBy);
    std::int64_t getLongMetric(const std::string& metricName);
    
protected:
    friend Bridge;
    
private:
    explicit Trace(Bridge* plugin, const std::string& name);
    
private:
    Bridge* plugin_;
    std::string name_;
};
} // namespace performance
} // namespace firebase
} // namespace ee

#endif /* EE_X_FIREBASE_PERFORMANCE_TRACE_HPP */
