//
//  Factory.cpp
//  ee_x
//
//  Created by eps on 3/20/18.
//

#include "ee/core/PluginManager.hpp"

#include <string>
#include <unordered_map>

#include "ee/core/LogLevel.hpp"
#include "ee/core/Platform.hpp"
#include "ee/core/Thread.hpp"
#include "ee/core/internal/MessageBridge.hpp"

#ifdef EE_X_ANDROID
#include "ee/core/internal/JniMethodInfo.hpp"
#include "ee/core/internal/JniString.hpp"
#include "ee/core/internal/JniUtils.hpp"
#endif // EE_X_ANDROID

namespace ee {
namespace core {
namespace {
std::unordered_map<Plugin, std::string> pluginNames_ = {{
    {Plugin::AdColony, "AdColony"},
    {Plugin::Adjust, "Adjust"},
    {Plugin::AdMob, "AdMob"},
    {Plugin::AppLovin, "AppLovin"},
    {Plugin::AppsFlyer, "AppsFlyer"},
    {Plugin::Facebook, "Facebook"},
    {Plugin::FacebookAds, "FacebookAds"},
    {Plugin::FirebaseCore, "FirebaseCore"},
    {Plugin::FirebaseCrashlytics, "FirebaseCrashlytics"},
    {Plugin::FirebasePerformance, "FirebasePerformance"},
    {Plugin::GoogleAnalytics, "GoogleAnalytics"},
    {Plugin::IronSource, "IronSource"},
    {Plugin::Notification, "Notification"},
    {Plugin::Play, "Play"},
    {Plugin::Recorder, "Recorder"},
    {Plugin::Store, "Store"},
    {Plugin::UnityAds, "UnityAds"},
    {Plugin::Vungle, "Vungle"},
}};

IMessageBridge* bridge_ = nullptr;
} // namespace

#if defined(EE_X_ANDROID)
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
void ee_staticInitializePlugins(const char* version);
void ee_staticSetLogLevel(int level);
void* ee_staticGetActivity();
void ee_staticSetActivity(void* activity);
void ee_staticAddPlugin(const char* name);
void ee_staticRemovePlugin(const char* name);
} // extern "C"
#endif // defined(EE_X_IOS) || defined(EE_X_OSX)
using Self = PluginManager;

template <>
void Self::initializePlugins<Library::Core>() {
    ee_staticInitializePlugins("2.2.3");
    bridge_ = &MessageBridge::getInstance();
    Platform::registerHandlers(*bridge_);

    // Default implementation: execute on the current thread.
    Thread::libraryThreadChecker_ = [] { //
        return true;
    };
    Thread::libraryThreadExecuter_ = [](const Runnable<>& runnable) {
        runnable();
        return true;
    };
}

IMessageBridge& Self::getBridge() {
    return *bridge_;
}

void Self::setLogLevel(const LogLevel& level) {
    ee_staticSetLogLevel(level.priority);
}

void* Self::getActivity() {
    return ee_staticGetActivity();
}

void Self::setActivity(void* activity) {
    ee_staticSetActivity(activity);
}

void Self::addPlugin(Plugin plugin) {
    auto&& name = pluginNames_.at(plugin);
    ee_staticAddPlugin(name.c_str());
}

void Self::removePlugin(Plugin plugin) {
    auto&& name = pluginNames_.at(plugin);
    ee_staticRemovePlugin(name.c_str());
}
} // namespace core
} // namespace ee
