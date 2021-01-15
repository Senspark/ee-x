//
//  JsbFirebaseLastFetchStatus.hpp
//  ee_x
//
//  Created by eps on 10/16/19
//

#ifndef EE_X_JSB_FIREBASE_LAST_FETCH_STATUS_HPP
#define EE_X_JSB_FIREBASE_LAST_FETCH_STATUS_HPP

#include <ee/FirebaseFwd.hpp>

namespace ee {
namespace firebase {
namespace remote_config {
bool registerJsbLastFetchStatus(se::Object* global);
} // namespace remote_config
} // namespace firebase
} // namespace ee

#endif // EE_X_JSB_FIREBASE_LAST_FETCH_STATUS_HPP