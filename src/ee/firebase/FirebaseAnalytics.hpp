//
//  FirebaseAnalytics.hpp
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#ifndef EE_X_FIREBASE_ANALYTICS_HPP_
#define EE_X_FIREBASE_ANALYTICS_HPP_

#include <string>

namespace ee {
namespace firebase {
/// Wrapper for Firebase Analytics.
/// Should have a single instance at a time.
/// https://firebase.google.com/docs/analytics/cpp/start
/// https://firebase.google.com/docs/reference/cpp/namespace/firebase/analytics
class FirebaseAnalytics {
public:
    FirebaseAnalytics();
    ~FirebaseAnalytics();

    /// Initializes the firebase analytics.
    /// @return True if successfully initialized, false otherwise.
    bool initialize();

    /// Set a user property to the given value.
    /// @param[in] name Name of the user property to set.
    /// @param[in] property Value to set the user property to.
    void setUserProperty(const std::string& name, const std::string& property);

private:
    bool initialized_;
};
} // namespace firebase
} // namespace ee

#endif /* EE_X_FIREBASE_ANALYTICS_HPP_ */
