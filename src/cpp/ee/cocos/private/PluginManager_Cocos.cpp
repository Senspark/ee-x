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
void Self::initializePlugins<Library::Cocos>() {
#if defined(EE_X_ANDROID)
    // Must set JavaVM and activity first.
    auto vm = cocos2d::JniHelper::getJavaVM();
    JniUtils::setVm(vm);

    auto activity = cocos2d::JniHelper::getActivity();
    setActivity(activity);
#endif // defined(EE_X_ANDROID)

    initializePlugins<Library::Core>();

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
}
} // namespace core
} // namespace ee
