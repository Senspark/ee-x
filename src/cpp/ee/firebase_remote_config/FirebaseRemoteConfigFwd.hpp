//
//  FirebaseRemoteConfigFwd.hpp
//  Pods
//
//  Created by eps on 2/2/21.
//

#ifndef EE_X_FIREBASE_REMOTE_CONFIG_FWD_HPP
#define EE_X_FIREBASE_REMOTE_CONFIG_FWD_HPP

#ifdef __cplusplus

#include <ee/core/CoreFwd.hpp>

namespace ee {
namespace firebase {
namespace remote_config {
enum class FetchStatus;
class IBridge;
class Bridge;
} // namespace remote_config
} // namespace firebase

using IFirebaseRemoteConfig = firebase::remote_config::IBridge;
using FirebaseFetchStatus = firebase::remote_config::FetchStatus;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_FIREBASE_REMOTE_CONFIG_FWD_HPP */
