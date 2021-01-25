#ifndef EE_X_JSB_FIREBASE_ANALYTICS_HPP
#define EE_X_JSB_FIREBASE_ANALYTICS_HPP

#ifdef __cplusplus

#include <ee/cocos/CocosFwd.hpp>

#ifdef EE_X_COCOS_JS

#include "ee/FirebaseFwd.hpp"

namespace ee {
namespace firebase {
namespace analytics {
bool registerJsb(se::Object* global);
} // namespace analytics
} // namespace firebase
} // namespace ee

#endif // EE_X_COCOS_JS
#endif // __cplusplus

#endif // EE_X_JSB_FIREBASE_ANALYTICS_HPP