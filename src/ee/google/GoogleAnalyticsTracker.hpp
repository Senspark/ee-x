//
//  GoogleAnalyticsTracker.hpp
//  ee_x
//
//  Created by Zinge on 10/24/17.
//
//

#ifndef EE_X_GOOGLE_ANALYTICS_TRACKER_HPP
#define EE_X_GOOGLE_ANALYTICS_TRACKER_HPP

#include <string>
#include <unordered_map>

namespace ee {
namespace google {
class Analytics;

class AnalyticsTracker {
public:
    ~AnalyticsTracker();

    void setAllowIDFACollection(bool enabled);

    void send(const std::unordered_map<std::string, std::string>& dict);

protected:
    friend Analytics;

    explicit AnalyticsTracker(Analytics* plugin, const std::string& trackingId);

private:
    Analytics* plugin_;
    std::string trackingId_;
};
} // namespace google
} // namespace ee

#endif /* EE_X_GOOGLE_ANALYTICS_TRACKER_HPP */
