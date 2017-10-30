//
//  GoogleAnalyticsTracker.hpp
//  ee_x
//
//  Created by Zinge on 10/24/17.
//
//

#ifndef EE_X_GOOGLE_ANALYTICS_TRACKER_HPP
#define EE_X_GOOGLE_ANALYTICS_TRACKER_HPP

#include "ee/GoogleAnalyticsFwd.hpp"

namespace ee {
namespace google {
class Analytics;

class AnalyticsTracker {
public:
    ~AnalyticsTracker();

    /// Sets the model value for the given key. All subsequent track or send
    /// calls will send this key-value pair along as well. To remove a
    /// particular field, simply set the value to null.
    /// @param key The key of the field that needs to be set. It starts with "&"
    /// followed by the parameter name. The complete list of fields can be found
    /// at https://goo.gl/M6dK2U.
    /// @param value A string value to be sent to Google servers. A null value
    /// denotes that the value should not be sent over wire.
    void setParameter(const std::string& key, const std::string& value);

    void setAllowIDFACollection(bool enabled);

    /// Set the screen name to be associated with all subsequent hits.
    void setScreenName(const std::string& screenName);

    void send(const TrackingDict& dict);

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
