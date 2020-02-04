//
//  FirebaseFwd.hpp
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#ifndef EE_X_FIREBASE_FWD_HPP
#define EE_X_FIREBASE_FWD_HPP

#include <ee/CoreFwd.hpp>

namespace ee {
namespace firebase {
class App;

#if defined(EE_X_MOBILE)
template <class T>
class Scheduler;
#endif // EE_X_MOBILE

namespace analytics {
class Bridge;
} // namespace analytics

namespace dynamic_link {
class Bridge;
} // namespace dynamic_link

namespace messaging {
class Notification;
class Message;
class Bridge;
} // namespace messaging

namespace performance {
class Bridge;
class Trace;
} // namespace performance

namespace remote_config {
enum class LastFetchStatus;
enum class FetchFailureReason;
struct ConfigInfo;
class Bridge;
} // namespace remote_config

namespace storage {
class Bridge;
} // namespace storage
} // namespace firebase

using FirebaseApp = firebase::App;
using FirebaseAnalytics = firebase::analytics::Bridge;
using FirebaseDynamicLink = firebase::dynamic_link::Bridge;
using FirebaseMessaging = firebase::messaging::Bridge;
using firebase::messaging::Message;
using FirebasePerformance = firebase::performance::Bridge;
using FirebasePerformanceTrace = firebase::performance::Trace;
using FirebaseLastFetchStatus = firebase::remote_config::LastFetchStatus;
using FirebaseFetchFailureReason = firebase::remote_config::FetchFailureReason;
using FirebaseConfigInfo = firebase::remote_config::ConfigInfo;
using FirebaseRemoteConfig = firebase::remote_config::Bridge;
using FirebaseStorage = firebase::storage::Bridge;
} // namespace ee

#endif /* EE_X_FIREBASE_FWD_HPP */
