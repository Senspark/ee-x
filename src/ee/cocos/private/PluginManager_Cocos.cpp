//
//  PluginManager_Cocos.cpp
//  Pods
//
//  Created by eps on 6/16/20.
//

#include <ee/core/PluginManager.hpp>

#include <thread>

#include <base/CCDirector.h>
#include <base/CCScheduler.h>

#if defined(EE_X_ANDROID)
#include <jni/JniHelper.h>

#include <ee/core/internal/JniUtils.hpp>
#endif // defined(EE_X_ANDROID)

#include <ee/core/Thread.hpp>

namespace ee {
namespace core {
using Self = PluginManager;

template <>
bool Self::initializePlugins<Library::Cocos>() {
    if (not initializePlugins<Library::Core>()) {
        return false;
    }
#if defined(EE_X_ANDROID)
    auto vm = cocos2d::JniHelper::getJavaVM();
    JniUtils::setVm(vm);
#endif // defined(EE_X_ANDROID)

    auto id = std::this_thread::get_id();
    Thread::libraryThreadChecker_ = [id] { //
        return id == std::this_thread::get_id();
    };
    Thread::libraryThreadExecuter_ = [id](const Runnable<>& runnable) {
        if (id == std::this_thread::get_id()) {
            runnable();
            return true;
        }
        auto director = cocos2d::Director::getInstance();
        auto scheduler = director->getScheduler();
        scheduler->performFunctionInCocosThread([runnable] { //
            runnable();
        });
        return false;
    };
    return true;
}
} // namespace core
} // namespace ee
