//
//  PluginManager_Cocos.cpp
//  Pods
//
//  Created by eps on 6/16/20.
//

#include <ee/core/PluginManager.hpp>

#if defined(EE_X_ANDROID)
#include <jni/JniHelper.h>

#include <ee/core/internal/JniUtils.hpp>
#endif // defined(EE_X_ANDROID)

namespace ee {
namespace core {
using Self = PluginManager;

template <>
bool Self::initializePlugins<Library::Cocos>() {
#if defined(EE_X_ANDROID)
    auto vm = cocos2d::JniHelper::getJavaVM();
    JniUtils::setVm(vm);
#endif // defined(EE_X_ANDROID)
    return initializePlugins<Library::Core>();
}
} // namespace core
} // namespace ee
