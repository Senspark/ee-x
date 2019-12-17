//
//  FirebaseAnalytics.hpp
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#ifndef EE_X_FIREBASE_ANALYTICS_HPP_
#define EE_X_FIREBASE_ANALYTICS_HPP_

#include <cstdint>
#include <string>
#include <unordered_map>

namespace ee {
namespace firebase {
using TrackingDict = std::unordered_map<std::string, std::string>;

/// Wrapper for Firebase Analytics.
/// Should have a single instance at a time.
/// https://firebase.google.com/docs/analytics/cpp/start
/// https://firebase.google.com/docs/reference/cpp/namespace/firebase/analytics
class Analytics {
public:
    Analytics();
    ~Analytics();

    /// Initializes the firebase analytics.
    /// @return True if successfully initialized, false otherwise.
    bool initialize();

    /// Sets whether analytics collection is enabled for this app on this
    /// device.
    /// This setting is persisted across app sessions. By default it is enabled.
    /// @param[in] enabled true to enable analytics collection, false to
    /// disable.
    void analyticsCollectionEnabled(bool enabled);

    /// @brief Sets the duration of inactivity that terminates the current
    /// session.
    ///
    /// @note The default value is 1800000 (30 minutes).
    ///
    /// @param milliseconds The duration of inactivity that terminates the
    /// current session.
    void setSessionTimeoutDuration(std::int64_t milliseconds);

    /// Sets the user ID property.
    /// This feature must be used in accordance with
    /// <a href="https://www.google.com/policies/privacy">Google's Privacy
    /// Policy</a>
    /// @param[in] userId The user ID associated with the user of this app on
    /// this device. The user ID must be non-empty and no more than 36
    /// characters long. Setting user_id to NULL or nullptr removes the user ID.
    void setUserId(const std::string& userId);

    /// Set a user property to the given value.
    /// @param[in] name Name of the user property to set.
    /// @param[in] property Value to set the user property to.
    void setUserProperty(const std::string& name, const std::string& property);

    /// Log an event with with associated parameters.
    /// @param[in] name Name of the event to log. Should contain 1 to 32
    /// alphanumeric characters or underscores. The name must start with an
    /// alphabetic character. Some event names are reserved.
    /// @param[in] dict Dictionary of Parameter structures.
    void logEvent(const std::string& name, const TrackingDict& dict = {});

private:
    bool initialized_;
};
} // namespace firebase
} // namespace ee

#endif /* EE_X_FIREBASE_ANALYTICS_HPP_ */
