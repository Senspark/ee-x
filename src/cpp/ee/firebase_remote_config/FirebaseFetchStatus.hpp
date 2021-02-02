//
//  FirebaseFetchStatus.hpp
//  Pods
//
//  Created by eps on 2/2/21.
//

#ifndef EE_X_FIREBASE_FETCH_STATUS_HPP
#define EE_X_FIREBASE_FETCH_STATUS_HPP

#ifdef __cplusplus

#include "ee/firebase_remote_config/FirebaseRemoteConfigFwd.hpp"

namespace ee {
namespace firebase {
namespace remote_config {
enum class FetchStatus {
    Success,
    NoFetchYet,
    Failure,
    Throttle,
};
} // namespace remote_config
} // namespace firebase
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_FIREBASE_FETCH_STATUS_HPP */
