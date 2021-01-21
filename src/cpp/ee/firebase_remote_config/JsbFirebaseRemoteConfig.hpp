#ifndef EE_X_JSB_FIREBASE_REMOTE_CONFIG_HPP
#define EE_X_JSB_FIREBASE_REMOTE_CONFIG_HPP

#ifdef __cplusplus

#include <ee/cocos/CocosFwd.hpp>

#ifdef EE_X_COCOS_JS

#include "ee/FirebaseFwd.hpp"

namespace ee {
namespace firebase {
namespace remote_config {
bool registerJsb(se::Object* global);
} // namespace remote_config
} // namespace firebase
} // namespace ee

#endif // EE_X_COCOS_JS
#endif // __cplusplus

#endif // EE_X_JSB_FIREBASE_REMOTE_CONFIG_HPP