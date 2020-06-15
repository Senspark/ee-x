//
//  Factory.hpp
//  ee_x
//
//  Created by eps on 3/20/18.
//

#ifndef EE_X_PLUGIN_MANAGER_HPP
#define EE_X_PLUGIN_MANAGER_HPP

#ifdef __cplusplus

#include <memory>

#include <ee/CoreFwd.hpp>

namespace ee {
namespace core {
enum class Plugin {
    AdMob,
    AppLovin,
    AppsFlyer,
    CampaignReceiver,
    Facebook,
    FacebookAds,
    FirebaseCore,
    FirebaseCrashlytics,
    FirebasePerformance,
    GoogleAnalytics,
    IronSource,
    Notification,
    Play,
    Recorder,
    Store,
    Tenjin,
    UnityAds,
    Vungle,
};

class PluginManager {
public:
    static void initializePlugins();

    template <class T>
    static std::unique_ptr<T> createPlugin();

    static bool addPlugin(Plugin plugin);
    static bool removePlugin(Plugin plugin);
};
} // namespace core
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_PLUGIN_MANAGER_HPP */
