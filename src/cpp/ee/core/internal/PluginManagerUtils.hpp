#ifndef EE_X_PLUGIN_MANAGER_UTILS_HPP
#define EE_X_PLUGIN_MANAGER_UTILS_HPP

#include <string>

#include "ee/core/CoreFwd.hpp"

namespace ee {
namespace core {
class PluginManagerUtils {
public:
    static void initializePlugins(const std::string& version);
    static void setLogLevel(int level);
    static void* getActivity();
    static void setActivity(void* activity);
    static void addPlugin(const std::string& name);
    static void removePlugin(const std::string& name);
};
} // namespace core
} // namespace ee

#endif // EE_X_PLUGIN_MANAGER_UTILS_HPP