//
//  PluginManager.cpp
//  ee_core
//
//  Created by Zinge on 3/29/17.
//
//

#include "ee/internal/PluginManager.hpp"
#include "ee/internal/MessageBridge.hpp"

namespace ee {
namespace core {
PluginManager& PluginManager::getInstance() {
    static PluginManager sharedInstance;
    return sharedInstance;
}

void PluginManager::addPlugin(const std::string& pluginName) {
    MessageBridge::getInstance().call("__plugin_manager_add_plugin",
                                      pluginName);
}

void PluginManager::removePlugin(const std::string& pluginName) {
    MessageBridge::getInstance().call("__plugin_manager_remove_plugin",
                                      pluginName);
}
} // namespace core
} // namespace ee
