//
//  jsb_firebase.hpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 8/22/18
//

#ifndef EE_X_JSB_FIREBASE_HPP
#define EE_X_JSB_FIREBASE_HPP

#include <ee/jsb/JsbCoreFwd.hpp>

namespace ee {
namespace firebase {
namespace analytics {
bool registerJsb(se::Object* global);
} // namespace analytics
namespace performance {
bool registerJsb(se::Object* global);
} // namespace performance
namespace remote_config {
bool registerJsb(se::Object* global);
} // namespace remote_config
} // namespace firebase
} // namespace ee

#endif // JSB_FIREBASE_HPP__
