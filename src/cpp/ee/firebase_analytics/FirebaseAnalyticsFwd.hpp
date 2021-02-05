//
//  FirebaseAnalyticsFwd.hpp
//  Pods
//
//  Created by eps on 2/1/21.
//

#ifndef EE_X_FIREBASE_ANALYTICS_FWD_HPP
#define EE_X_FIREBASE_ANALYTICS_FWD_HPP

#ifdef __cplusplus

#include <ee/core/CoreFwd.hpp>

namespace ee {
namespace firebase {
namespace analytics {
class IBridge;
class Bridge;
} // namespace analytics
} // namespace firebase

using IFirebaseAnalytics = firebase::analytics::IBridge;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_FIREBASE_ANALYTICS_FWD_HPP */
