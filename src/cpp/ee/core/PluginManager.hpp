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

#include "ee/core/CoreFwd.hpp"

namespace ee {
namespace core {
enum class Plugin {
    AdColony,
    Adjust,
    AdMob,
    AppLovin,
    AppsFlyer,
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
    UnityAds,
    Vungle,
};

class PluginManager {
public:
    /// Initializes plugins.
    /// Must be called first.
    static void initializePlugins();

    /// Gets the current message bridge.
    static IMessageBridge& getBridge();

    template <class T>
    static std::shared_ptr<T> createPlugin();

    /// Sets the log level for the logger on Android/iOS.
    /// @param level The desired log level.
    static void setLogLevel(const LogLevel& level);

    /// Gets the current activity.
    static void* getActivity();

    /// Sets the current activity.
    static void setActivity(void* activity);

private:
    static void addPlugin(Plugin plugin);
    static void removePlugin(Plugin plugin);

    static std::shared_ptr<IPluginManagerImpl> impl_;
    static std::shared_ptr<ILogger> logger_;
};
} // namespace core
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_PLUGIN_MANAGER_HPP */
