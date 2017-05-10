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
class FirebaseAnalytics {
public:
    FirebaseAnalytics();
    ~FirebaseAnalytics();

    bool initialize();

    void setUserProperty(const std::string& name, const std::string& property);

private:
    bool initialized_;
};
} // namespace firebase
} // namespace ee

#endif /* EE_X_FIREBASE_ANALYTICS_HPP_ */
