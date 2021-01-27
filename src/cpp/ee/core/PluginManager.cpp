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
#include <ee/firebase_crashlytics/private/FirebaseCrashlyticsBridge.hpp>
#include <ee/firebase_performance/private/FirebasePerformanceBridge.hpp>
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
    static constexpr auto Plugin = Plugin::AdColony;
};

template <>
struct PluginInfo<IAdjust> {
    using Bridge = adjust::Bridge;
    static constexpr auto Name = "Adjust";
    static constexpr auto Plugin = Plugin::Adjust;
};

template <>
struct PluginInfo<IAdMob> {
    using Bridge = ad_mob::Bridge;
    static constexpr auto Name = "AdMob";
    static constexpr auto Plugin = Plugin::AdMob;
};

template <>
struct PluginInfo<IAppLovin> {
    using Bridge = app_lovin::Bridge;
    static constexpr auto Name = "AppLovin";
    static constexpr auto Plugin = Plugin::AppLovin;
};

template <>
struct PluginInfo<IAppsFlyer> {
    using Bridge = apps_flyer::Bridge;
    static constexpr auto Name = "AppsFlyer";
    static constexpr auto Plugin = Plugin::AppsFlyer;
};

template <>
struct PluginInfo<IFacebook> {
    using Bridge = facebook::Bridge;
    static constexpr auto Name = "Facebook";
    static constexpr auto Plugin = Plugin::Facebook;
};

template <>
struct PluginInfo<IFacebookAds> {
    using Bridge = facebook_ads::Bridge;
    static constexpr auto Name = "FacebookAds";
    static constexpr auto Plugin = Plugin::FacebookAds;
};

template <>
struct PluginInfo<IFirebaseCrashlytics> {
    using Bridge = firebase::crashlytics::Bridge;
    static constexpr auto Name = "FirebaseCrashlytics";
    static constexpr auto Plugin = Plugin::FirebaseCrashlytics;
};

template <>
struct PluginInfo<IFirebasePerformance> {
    using Bridge = firebase::performance::Bridge;
    static constexpr auto Name = "FirebasePerformance";
    static constexpr auto Plugin = Plugin::FirebasePerformance;
};

template <>
struct PluginInfo<IIronSource> {
    using Bridge = iron_source::Bridge;
    static constexpr auto Name = "IronSource";
    static constexpr auto Plugin = Plugin::IronSource;
};

template <>
struct PluginInfo<INotification> {
    using Bridge = notification::Bridge;
    static constexpr auto Name = "Notification";
    static constexpr auto Plugin = Plugin::Notification;
};

template <>
struct PluginInfo<IPlay> {
    using Bridge = play::Bridge;
    static constexpr auto Name = "Play";
    static constexpr auto Plugin = Plugin::Play;
};

template <>
struct PluginInfo<IStore> {
    using Bridge = store::Bridge;
    static constexpr auto Name = "Store";
    static constexpr auto Plugin = Plugin::Store;
};

template <>
struct PluginInfo<IUnityAds> {
    using Bridge = unity_ads::Bridge;
    static constexpr auto Name = "UnityAds";
    static constexpr auto Plugin = Plugin::UnityAds;
};

template <>
struct PluginInfo<IVungle> {
    using Bridge = vungle::Bridge;
    static constexpr auto Name = "Vungle";
    static constexpr auto Plugin = Plugin::Vungle;
};

using Self = PluginManager;

std::shared_ptr<IPluginManagerImpl> Self::impl_;
std::shared_ptr<Logger> Self::logger_;

void Self::initializePlugins() {
#if defined(EE_X_ANDROID)
    // Must set JavaVM and activity first.
    auto vm = cocos2d::JniHelper::getJavaVM();
    JniUtils::setVm(vm);

    auto activity = cocos2d::JniHelper::getActivity();
    setActivity(activity);
#endif // defined(EE_X_ANDROID)

    PluginManagerUtils::initializePlugins("2.3.1");
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
    auto instance =
        std::make_shared<Bridge>(getBridge(), *logger_, [plugin] { //
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

template std::shared_ptr<IAdColony> Self::createPlugin<IAdColony>();
template std::shared_ptr<IAdjust> Self::createPlugin<IAdjust>();
template std::shared_ptr<IAdMob> Self::createPlugin<IAdMob>();
template std::shared_ptr<IAppLovin> Self::createPlugin<IAppLovin>();
template std::shared_ptr<IAppsFlyer> Self::createPlugin<IAppsFlyer>();
template std::shared_ptr<IFacebook> Self::createPlugin<IFacebook>();
template std::shared_ptr<IFacebookAds> Self::createPlugin<IFacebookAds>();
template std::shared_ptr<IFirebaseCrashlytics>
Self::createPlugin<IFirebaseCrashlytics>();
template std::shared_ptr<IFirebasePerformance>
Self::createPlugin<IFirebasePerformance>();
template std::shared_ptr<IIronSource> Self::createPlugin<IIronSource>();
template std::shared_ptr<INotification> Self::createPlugin<INotification>();
template std::shared_ptr<IPlay> Self::createPlugin<IPlay>();
template std::shared_ptr<IStore> Self::createPlugin<IStore>();
template std::shared_ptr<IUnityAds> Self::createPlugin<IUnityAds>();
template std::shared_ptr<IVungle> Self::createPlugin<IVungle>();
} // namespace core
} // namespace ee
