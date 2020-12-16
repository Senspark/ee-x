//
//  FirebaseApp.cpp
//  ee_firebase
//
//  Created by Zinge on 5/9/17.
//
//

#include "ee/firebase_core/FirebaseCoreBridge.hpp"

#include <firebase/app.h>

#include <ee/core/PluginManager.hpp>

#ifdef EE_X_ANDROID
#include <ee/core/internal/JniUtils.hpp>
#endif // EE_X_ANDROID

namespace ee {
namespace firebase {
namespace core {
using Self = Bridge;

void Self::initialize() {
    static bool initialized = false;
    if (initialized) {
        return;
    }

    PluginManager::addPlugin(Plugin::FirebaseCore);

    auto options = ::firebase::AppOptions();

#ifdef EE_X_ANDROID
    auto env = JniUtils::getEnv();
    auto activity = PluginManager::getActivity();
    [[maybe_unused]] auto app =
        ::firebase::App::Create(options, env, static_cast<jobject>(activity));
#endif // EE_X_ANDROID
#if defined(EE_X_IOS)
    [[maybe_unused]] auto app = ::firebase::App::Create(options);
#endif // defined(EE_X_IOS)

    initialized = true;
}
} // namespace core
} // namespace firebase
} // namespace ee
