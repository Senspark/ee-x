//
//  Factory.cpp
//  ee_x
//
//  Created by eps on 3/20/18.
//

#include "ee/core/PluginManager.hpp"

#include <string>
#include <unordered_map>

#include "ee/core/Platform.hpp"
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
    {Plugin::AdMob, "AdMob"},
    {Plugin::AppLovin, "AppLovin"},
    {Plugin::AppsFlyer, "AppsFlyer"},
    {Plugin::CampaignReceiver, "CampaignReceiver"},
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
    {Plugin::Tenjin, "Tenjin"},
    {Plugin::UnityAds, "UnityAds"},
    {Plugin::Vungle, "Vungle"},
}};
} // namespace

#if defined(EE_X_ANDROID)
bool ee_staticInitializePlugins() {
    auto method = JniUtils::getStaticMethodInfo(
        "com/ee/core/PluginManagerKt", "ee_staticInitializePlugins", "()Z");
    return method->getEnv()->CallStaticBooleanMethod(method->getClass(),
                                                     method->getMethodId());
}

void* ee_staticGetActivity() {
    auto method = JniUtils::getStaticMethodInfo("com/ee/core/PluginManagerKt",
                                                "ee_staticGetActivity",
                                                "()Ljava/lang/Object;");
    return method->getEnv()->CallStaticObjectMethod(method->getClass(),
                                                    method->getMethodId());
}

bool ee_staticAddPlugin(const char* name) {
    auto method = JniUtils::getStaticMethodInfo("com/ee/core/PluginManagerKt",
                                                "ee_staticAddPlugin",
                                                "(Ljava/lang/String;)Z");
    auto name_java = JniUtils::toJavaString(name);
    return method->getEnv()->CallStaticBooleanMethod(
        method->getClass(), method->getMethodId(), name_java->get());
}

bool ee_staticRemovePlugin(const char* name) {
    auto method = JniUtils::getStaticMethodInfo("com/ee/core/PluginManagerKt",
                                                "ee_staticRemovePlugin",
                                                "(Ljava/lang/String;)Z");
    auto name_java = JniUtils::toJavaString(name);
    return method->getEnv()->CallStaticBooleanMethod(
        method->getClass(), method->getMethodId(), name_java->get());
}
#endif // EE_X_ANDROID

#if defined(EE_X_IOS) || defined(EE_X_OSX)
extern "C" {
bool ee_staticInitializePlugins();
void* ee_staticGetActivity();
bool ee_staticAddPlugin(const char* name);
bool ee_staticRemovePlugin(const char* name);
} // extern "C"
#endif // defined(EE_X_IOS) || defined(EE_X_OSX)
using Self = PluginManager;

template <>
bool Self::initializePlugins<Library::Core>() {
    auto&& bridge = MessageBridge::getInstance();
    Platform::registerHandlers(bridge);
    return ee_staticInitializePlugins();
}

void* Self::getActivity() {
    return ee_staticGetActivity();
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
