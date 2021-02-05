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
class PluginManager {
public:
    /// Initializes plugins.
    /// Must be called first.
    static void initializePlugins();

    template <class T>
    static std::shared_ptr<T> createPlugin();

    /// Gets the current message bridge.
    static IMessageBridge& getBridge();

    /// Gets the current logger.
    static ILogger& getLogger();

    /// Sets the log level for the logger on Android/iOS.
    /// @param level The desired log level.
    static void setLogLevel(LogLevel level);

    /// Gets the current activity.
    static void* getActivity();

    /// Sets the current activity.
    static void setActivity(void* activity);

private:
    static std::shared_ptr<IPluginManagerImpl> impl_;
    static std::shared_ptr<Logger> logger_;
};
} // namespace core
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_PLUGIN_MANAGER_HPP */
