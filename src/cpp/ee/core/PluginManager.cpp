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
#include "ee/core/internal/PluginManagerImplCpp.hpp"
#include "ee/core/internal/PluginManagerImplJs.hpp"

#ifdef EE_X_ANDROID
#include <jni/JniHelper.h>

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
} // namespace

template <class T>
struct PluginParser;

template <>
struct PluginParser<IAdColony> {
    using Bridge = ad_colony::Bridge;
    static constexpr auto Plugin = Plugin::AdColony;
};

template <>
struct PluginParser<IAdjust> {
    using Bridge = adjust::Bridge;
    static constexpr auto Plugin = Plugin::Adjust;
};

template <>
struct PluginParser<IAdMob> {
    using Bridge = ad_mob::Bridge;
    static constexpr auto Plugin = Plugin::AdMob;
};

template <>
struct PluginParser<IAppLovin> {
    using Bridge = app_lovin::Bridge;
    static constexpr auto Plugin = Plugin::AppLovin;
};

template <>
struct PluginParser<IAppsFlyer> {
    using Bridge = apps_flyer::Bridge;
    static constexpr auto Plugin = Plugin::AppsFlyer;
};

template <>
struct PluginParser<IFacebook> {
    using Bridge = facebook::Bridge;
    static constexpr auto Plugin = Plugin::Facebook;
};

template <>
struct PluginParser<IFacebookAds> {
    using Bridge = facebook_ads::Bridge;
    static constexpr auto Plugin = Plugin::FacebookAds;
};

template <>
struct PluginParser<IFirebaseCrashlytics> {
    using Bridge = firebase::crashlytics::Bridge;
    static constexpr auto Plugin = Plugin::FirebaseCrashlytics;
};

template <>
struct PluginParser<IFirebasePerformance> {
    using Bridge = firebase::performance::Bridge;
    static constexpr auto Plugin = Plugin::FirebasePerformance;
};

template <>
struct PluginParser<IIronSource> {
    using Bridge = iron_source::Bridge;
    static constexpr auto Plugin = Plugin::IronSource;
};

template <>
struct PluginParser<INotification> {
    using Bridge = notification::Bridge;
    static constexpr auto Plugin = Plugin::Notification;
};

template <>
struct PluginParser<IPlay> {
    using Bridge = play::Bridge;
    static constexpr auto Plugin = Plugin::Play;
};

template <>
struct PluginParser<IStore> {
    using Bridge = store::Bridge;
    static constexpr auto Plugin = Plugin::Store;
};

template <>
struct PluginParser<IUnityAds> {
    using Bridge = unity_ads::Bridge;
    static constexpr auto Plugin = Plugin::UnityAds;
};

template <>
struct PluginParser<IVungle> {
    using Bridge = vungle::Bridge;
    static constexpr auto Plugin = Plugin::Vungle;
};

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

using Self = PluginManager;

std::shared_ptr<IPluginManagerImpl> Self::impl_;
std::shared_ptr<ILogger> Self::logger_;

void Self::initializePlugins() {
#if defined(EE_X_ANDROID)
    // Must set JavaVM and activity first.
    auto vm = cocos2d::JniHelper::getJavaVM();
    JniUtils::setVm(vm);

    auto activity = cocos2d::JniHelper::getActivity();
    setActivity(activity);
#endif // defined(EE_X_ANDROID)

    ee_staticInitializePlugins("2.3.1");
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

IMessageBridge& Self::getBridge() {
    return impl_->getBridge();
}

template <class T>
std::shared_ptr<T> Self::createPlugin() {
    auto&& plugin = PluginParser<T>::Plugin;
    addPlugin(plugin);
    using Bridge = typename PluginParser<T>::Bridge;
    auto instance =
        std::make_shared<Bridge>(getBridge(), *logger_, [plugin] { //
            removePlugin(plugin);
        });
    return instance;
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
