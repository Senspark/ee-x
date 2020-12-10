//
//  JsbFirebaseFetchFailureReason.hpp
//  ee_x
//
//  Created by eps on 10/16/19
//

#ifndef EE_X_JSB_FIREBASE_FETCH_FAILURE_REASON_HPP
#define EE_X_JSB_FIREBASE_FETCH_FAILURE_REASON_HPP

#include <ee/jsb/JsbCoreFwd.hpp>

namespace ee {
namespace firebase {
namespace remote_config {
bool registerJsbFetchFailureReason(se::Object* global);
} // namespace remote_config
} // namespace firebase
} // namespace ee

#endif // EE_X_JSB_FIREBASE_FETCH_FAILURE_REASON_HPP