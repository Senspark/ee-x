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
bool ee_staticInitializePlugins(const char* version) {
    return JniUtils::callStaticBooleanMethod( //
        "com/ee/PluginManagerKt",             //
        "ee_staticInitializePlugins",         //
        "(Ljava/lang/String;)Z",              //
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

bool ee_staticAddPlugin(const char* name) {
    return JniUtils::callStaticBooleanMethod(
        "com/ee/PluginManagerKt", //
        "ee_staticAddPlugin",     //
        "(Ljava/lang/String;)Z",  //
        JniUtils::toJavaString(name)->get());
}

bool ee_staticRemovePlugin(const char* name) {
    return JniUtils::callStaticBooleanMethod(
        "com/ee/PluginManagerKt", //
        "ee_staticRemovePlugin",  //
        "(Ljava/lang/String;)Z",  //
        JniUtils::toJavaString(name)->get());
}
#endif // EE_X_ANDROID

#if defined(EE_X_IOS) || defined(EE_X_OSX)
extern "C" {
bool ee_staticInitializePlugins(const char* version);
void ee_staticSetLogLevel(int level);
void* ee_staticGetActivity();
void ee_staticSetActivity(void* activity);
bool ee_staticAddPlugin(const char* name);
bool ee_staticRemovePlugin(const char* name);
} // extern "C"
#endif // defined(EE_X_IOS) || defined(EE_X_OSX)
using Self = PluginManager;

template <>
bool Self::initializePlugins<Library::Core>() {
    if (not ee_staticInitializePlugins("2.1.2")) {
        return false;
    }
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
    return true;
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

bool Self::addPlugin(Plugin plugin) {
    auto&& name = pluginNames_.at(plugin);
    return ee_staticAddPlugin(name.c_str());
}

bool Self::removePlugin(Plugin plugin) {
    auto&& name = pluginNames_.at(plugin);
    return ee_staticRemovePlugin(name.c_str());
}
} // namespace core
} // namespace ee
