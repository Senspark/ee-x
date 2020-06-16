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

namespace ee {
namespace core {
using Self = PluginManager;

template <>
bool Self::initializePlugins<Library::Cocos>() {
    auto vm = cocos2d::JniHelper::getJavaVM();
    JniUtils::setVm(vm);
    initializePlugins<Library::Core>();
}
} // namespace core
} // namespace ee
#endif // defined(EE_X_ANDROID)
