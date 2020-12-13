//
//  FirebaseFwd.hpp
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#ifndef EE_X_FIREBASE_FWD_HPP
#define EE_X_FIREBASE_FWD_HPP

#ifdef __cplusplus

#include <ee/CppFwd.hpp>

namespace ee {
namespace firebase {
namespace core {
class Bridge;
} // namespace core

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

using FirebaseCore = firebase::core::Bridge;
using FirebaseAnalytics = firebase::analytics::Bridge;
using FirebaseDynamicLink = firebase::dynamic_link::Bridge;
using FirebaseMessaging = firebase::messaging::Bridge;
using firebase::messaging::Message;
using FirebaseLastFetchStatus = firebase::remote_config::LastFetchStatus;
using FirebaseFetchFailureReason = firebase::remote_config::FetchFailureReason;
using FirebaseConfigInfo = firebase::remote_config::ConfigInfo;
using FirebaseRemoteConfig = firebase::remote_config::Bridge;
using FirebaseStorage = firebase::storage::Bridge;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_FIREBASE_FWD_HPP */
