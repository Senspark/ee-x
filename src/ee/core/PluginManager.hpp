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
enum class Library {
    Core,
    Cocos,
};

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
    /// Initializes plugins.
    /// Must be called first.
    template <Library library = Library::Core>
    static bool initializePlugins();

    template <class T>
    static std::unique_ptr<T> createPlugin();

    /// Adds a cross-platform plugin.
    /// @note Internal uses.
    static bool addPlugin(Plugin plugin);

    /// Remove a cross-platform plugin.
    /// @note Internal uses.
    static bool removePlugin(Plugin plugin);
};
} // namespace core
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_PLUGIN_MANAGER_HPP */
