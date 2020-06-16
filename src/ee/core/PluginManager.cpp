//
//  Factory.cpp
//  ee_x
//
//  Created by eps on 3/20/18.
//

#include "ee/core/PluginManager.hpp"

#include <string>
#include <unordered_map>

#include "ee/core/PlatformUtils.hpp"
#include "ee/core/internal/MessageBridge.hpp"

namespace ee {
namespace core {
using Self = PluginManager;

#if defined(EE_X_ANDROID)
template <>
bool Self::initializePlugins<Library::Core>() {
    // FIXME.
    return false;
}

bool Self::addPlugin(Plugin plugin) {
    // FIXME.
    return false;
}

bool Self::removePlugin(Plugin plugin) {
    // FIXME.
    return false;
}
#endif // EE_X_ANDROID

#if defined(EE_X_IOS) || defined(EE_X_OSX)
extern "C" {
bool ee_staticInitializePlugins();
bool ee_staticAddPlugin(const std::string& name);
bool ee_staticRemovePlugin(const std::string& name);
} // extern "C"

template <>
bool Self::initializePlugins<Library::Core>() {
    auto&& bridge = MessageBridge::getInstance();
    PlatformUtils::registerHandlers(bridge);
    return ee_staticInitializePlugins();
}

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

bool Self::addPlugin(Plugin plugin) {
    auto&& name = pluginNames_.at(plugin);
    return ee_staticAddPlugin(name);
}

bool Self::removePlugin(Plugin plugin) {
    auto&& name = pluginNames_.at(plugin);
    return ee_staticRemovePlugin(name);
}
#endif // defined(EE_X_IOS) || defined(EE_X_OSX)
} // namespace core
} // namespace ee
