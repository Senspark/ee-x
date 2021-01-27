#include "ee/core/internal/PluginManagerUtils.hpp"

#include "ee/core/IMessageBridge.hpp"
#include "ee/core/PluginManager.hpp"
#include "ee/core/Thread.hpp"

#ifdef EE_X_ANDROID
#include <jni/JniHelper.h>

#include "ee/core/internal/JniMethodInfo.hpp"
#include "ee/core/internal/JniString.hpp"
#include "ee/core/internal/JniUtils.hpp"
#endif // EE_X_ANDROID

namespace ee {
namespace core {
#if defined(EE_X_ANDROID)
extern "C" {
JNIEXPORT void JNICALL Java_com_ee_PluginManagerKt_ee_1callCppInternal(
    JNIEnv* env, jclass clazz, jstring tag, jstring message) {
    auto tag_cpp = env->GetStringUTFChars(tag, nullptr);
    auto message_cpp = env->GetStringUTFChars(message, nullptr);
    std::string tagStr = tag_cpp;
    std::string messageStr = message_cpp;
    env->ReleaseStringUTFChars(tag, tag_cpp);
    env->ReleaseStringUTFChars(message, message_cpp);
    Thread::runOnLibraryThread([tagStr, messageStr] { //
        PluginManager::getBridge().callCpp(tagStr, messageStr);
    });
}
} // extern "C"

void ee_staticInitializePlugins(const char* version) {
    JniUtils::callStaticVoidMethod(   //
        "com/ee/PluginManagerKt",     //
        "ee_staticInitializePlugins", //
        "(Ljava/lang/String;)V",      //
        JniUtils::toJavaString(version)->get());
}

void ee_staticSetLogLevel(int level) {
    JniUtils::callStaticVoidMethod( //
        "com/ee/PluginManagerKt",   //
        "ee_staticSetLogLevel",     //
        "(I)V",                     //
        level);
}

void* ee_staticGetActivity() {
    return JniUtils::callStaticObjectMethod( //
        "com/ee/PluginManagerKt",            //
        "ee_staticGetActivity",              //
        "()Ljava/lang/Object;");
}

void ee_staticSetActivity(void* activity) {
    JniUtils::callStaticVoidMethod( //
        "com/ee/PluginManagerKt",   //
        "ee_staticSetActivity",     //
        "(Ljava/lang/Object;)V",    //
        activity);
}

void ee_staticAddPlugin(const char* name) {
    JniUtils::callStaticVoidMethod("com/ee/PluginManagerKt", //
                                   "ee_staticAddPlugin",     //
                                   "(Ljava/lang/String;)V",  //
                                   JniUtils::toJavaString(name)->get());
}

void ee_staticRemovePlugin(const char* name) {
    JniUtils::callStaticVoidMethod("com/ee/PluginManagerKt", //
                                   "ee_staticRemovePlugin",  //
                                   "(Ljava/lang/String;)V",  //
                                   JniUtils::toJavaString(name)->get());
}
#endif // EE_X_ANDROID

#if defined(EE_X_IOS) || defined(EE_X_OSX)
extern "C" {
void ee_callCppInternal(const char* tag, const char* message) {
    std::string tagStr = tag;
    std::string messageStr = message;
    Thread::runOnLibraryThread([tagStr, messageStr] { //
        PluginManager::getBridge().callCpp(tagStr, messageStr);
    });
}

void ee_staticInitializePlugins(const char* version);
void ee_staticSetLogLevel(int level);
void* ee_staticGetActivity();
void ee_staticSetActivity(void* activity);
void ee_staticAddPlugin(const char* name);
void ee_staticRemovePlugin(const char* name);
} // extern "C"
#endif // defined(EE_X_IOS) || defined(EE_X_OSX)

using Self = PluginManagerUtils;

void Self::initializePlugins(const std::string& version) {
#if defined(EE_X_ANDROID)
    // Must set JavaVM and activity first.
    auto vm = cocos2d::JniHelper::getJavaVM();
    JniUtils::setVm(vm);

    auto activity = cocos2d::JniHelper::getActivity();
    setActivity(activity);
#endif // defined(EE_X_ANDROID)

    ee_staticInitializePlugins(version.c_str());
}

void Self::setLogLevel(int level) {
    ee_staticSetLogLevel(level);
}

void* Self::getActivity() {
    return ee_staticGetActivity();
}

void Self::setActivity(void* activity) {
    ee_staticSetActivity(activity);
}

void Self::addPlugin(const std::string& name) {
    ee_staticAddPlugin(name.c_str());
}

void Self::removePlugin(const std::string& name) {
    ee_staticRemovePlugin(name.c_str());
}
} // namespace core
} // namespace ee