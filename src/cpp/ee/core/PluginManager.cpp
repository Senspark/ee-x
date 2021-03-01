//
//  Factory.cpp
//  ee_x
//
//  Created by eps on 3/20/18.
//

#include "ee/core/PluginManager.hpp"

#include <string>
#include <unordered_map>

#include <ee/ad_colony/private/AdColonyBridge.hpp>
#include <ee/ad_mob/private/AdMobBridge.hpp>
#include <ee/adjust/private/AdjustBridge.hpp>
#include <ee/app_lovin/private/AppLovinBridge.hpp>
#include <ee/apps_flyer/private/AppsFlyerBridge.hpp>
#include <ee/cocos/CocosFwd.hpp>
#include <ee/facebook/private/FacebookBridge.hpp>
#include <ee/facebook_ads/private/FacebookAdsBridge.hpp>
#include <ee/firebase_analytics/internal/FirebaseAnalyticsBridge.hpp>
#include <ee/firebase_crashlytics/private/FirebaseCrashlyticsBridge.hpp>
#include <ee/firebase_performance/private/FirebasePerformanceBridge.hpp>
#include <ee/firebase_remote_config/internal/FirebaseRemoteConfigBridge.hpp>
#include <ee/iron_source/private/IronSourceBridge.hpp>
#include <ee/notification/internal/NotificationBridge.hpp>
#include <ee/play/private/PlayBridge.hpp>
#include <ee/store/private/StoreBridge.hpp>
#include <ee/unity_ads/private/UnityAdsBridge.hpp>
#include <ee/vungle/private/VungleBridge.hpp>

#include "ee/core/IMessageBridge.hpp"
#include "ee/core/LogLevel.hpp"
#include "ee/core/Logger.hpp"
#include "ee/core/Platform.hpp"
#include "ee/core/Thread.hpp"
#include "ee/core/Utils.hpp"
#include "ee/core/internal/PluginManagerImplCpp.hpp"
#include "ee/core/internal/PluginManagerImplJs.hpp"
#include "ee/core/internal/PluginManagerUtils.hpp"

namespace ee {
namespace core {
template <class T>
struct PluginInfo;

template <>
struct PluginInfo<IAdColony> {
    using Bridge = ad_colony::Bridge;
    static constexpr auto Name = "AdColony";
};

template <>
struct PluginInfo<IAdjust> {
    using Bridge = adjust::Bridge;
    static constexpr auto Name = "Adjust";
};

template <>
struct PluginInfo<IAdMob> {
    using Bridge = ad_mob::Bridge;
    static constexpr auto Name = "AdMob";
};

template <>
struct PluginInfo<IAppLovin> {
    using Bridge = app_lovin::Bridge;
    static constexpr auto Name = "AppLovin";
};

template <>
struct PluginInfo<IAppsFlyer> {
    using Bridge = apps_flyer::Bridge;
    static constexpr auto Name = "AppsFlyer";
};

template <>
struct PluginInfo<IFacebook> {
    using Bridge = facebook::Bridge;
    static constexpr auto Name = "Facebook";
};

template <>
struct PluginInfo<IFacebookAds> {
    using Bridge = facebook_ads::Bridge;
    static constexpr auto Name = "FacebookAds";
};

template <>
struct PluginInfo<IFirebaseAnalytics> {
    using Bridge = firebase::analytics::Bridge;
    static constexpr auto Name = "FirebaseAnalytics";
};

template <>
struct PluginInfo<IFirebaseCrashlytics> {
    using Bridge = firebase::crashlytics::Bridge;
    static constexpr auto Name = "FirebaseCrashlytics";
};

template <>
struct PluginInfo<IFirebasePerformance> {
    using Bridge = firebase::performance::Bridge;
    static constexpr auto Name = "FirebasePerformance";
};

template <>
struct PluginInfo<IFirebaseRemoteConfig> {
    using Bridge = firebase::remote_config::Bridge;
    static constexpr auto Name = "FirebaseRemoteConfig";
};

template <>
struct PluginInfo<IIronSource> {
    using Bridge = iron_source::Bridge;
    static constexpr auto Name = "IronSource";
};

template <>
struct PluginInfo<INotification> {
    using Bridge = notification::Bridge;
    static constexpr auto Name = "Notification";
};

template <>
struct PluginInfo<IPlay> {
    using Bridge = play::Bridge;
    static constexpr auto Name = "Play";
};

template <>
struct PluginInfo<IStore> {
    using Bridge = store::Bridge;
    static constexpr auto Name = "Store";
};

template <>
struct PluginInfo<IUnityAds> {
    using Bridge = unity_ads::Bridge;
    static constexpr auto Name = "UnityAds";
};

template <>
struct PluginInfo<IVungle> {
    using Bridge = vungle::Bridge;
    static constexpr auto Name = "Vungle";
};

using Self = PluginManager;

std::shared_ptr<IPluginManagerImpl> Self::impl_;
std::shared_ptr<Logger> Self::logger_;

void Self::initializePlugins() {
    PluginManagerUtils::initializePlugins("2.6.2");
    logger_ = std::make_shared<Logger>("ee-x");

#ifdef EE_X_COCOS_CPP
    impl_ = std::make_shared<PluginManagerImplCpp>(*logger_);
#else  // EE_X_COCOS_CPP
    impl_ = std::make_shared<PluginManagerImplJs>();
#endif // EE_X_COCOS_CPP

    impl_->initialize();
    Thread::initialize();
    Platform::initialize(impl_->getBridge());
}

template <class T>
std::shared_ptr<T> Self::createPlugin() {
    auto&& name = PluginInfo<T>::Name;
    PluginManagerUtils::addPlugin(name);
    using Bridge = typename PluginInfo<T>::Bridge;
    auto instance = std::make_shared<Bridge>(getBridge(), *logger_, [] { //
        PluginManagerUtils::removePlugin(name);
    });
    return instance;
}

IMessageBridge& Self::getBridge() {
    return impl_->getBridge();
}

ILogger& Self::getLogger() {
    return *logger_;
}

void Self::setLogLevel(LogLevel level) {
    auto&& priority = getLogLevelPriority(level);
    PluginManagerUtils::setLogLevel(priority);
}

void* Self::getActivity() {
    return PluginManagerUtils::getActivity();
}

void Self::setActivity(void* activity) {
    PluginManagerUtils::setActivity(activity);
}

#define EE_DECLARE_PLUGIN(T)                                                   \
    template std::shared_ptr<T> Self::createPlugin<T>();

EE_DECLARE_PLUGIN(IAdColony)
EE_DECLARE_PLUGIN(IAdjust)
EE_DECLARE_PLUGIN(IAdMob)
EE_DECLARE_PLUGIN(IAppLovin)
EE_DECLARE_PLUGIN(IAppsFlyer)
EE_DECLARE_PLUGIN(IFacebook)
EE_DECLARE_PLUGIN(IFacebookAds)
EE_DECLARE_PLUGIN(IFirebaseAnalytics)
EE_DECLARE_PLUGIN(IFirebaseCrashlytics)
EE_DECLARE_PLUGIN(IFirebasePerformance)
EE_DECLARE_PLUGIN(IFirebaseRemoteConfig)
EE_DECLARE_PLUGIN(IIronSource)
EE_DECLARE_PLUGIN(INotification)
EE_DECLARE_PLUGIN(IPlay)
EE_DECLARE_PLUGIN(IStore)
EE_DECLARE_PLUGIN(IUnityAds)
EE_DECLARE_PLUGIN(IVungle)

#undef EE_DECLARE_PLUGIN
} // namespace core
} // namespace ee
