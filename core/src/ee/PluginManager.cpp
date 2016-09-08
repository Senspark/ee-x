//
//  PluginManager.cpp
//  ee-core
//
//  Created by Zinge on 7/1/16.
//
//

#include "ee/PluginManager.hpp"
#include "ee/PluginProtocol.hpp"

namespace ee {
namespace core {
std::unordered_map<std::string, PluginProtocol*> PluginManager::plugins_{};

PluginProtocol* PluginManager::getPluginByName(const std::string& pluginName) {
    if (plugins_.count(pluginName)) {
        return plugins_.at(pluginName);
    }
    return nullptr;
}

void PluginManager::onNativeCallback(const std::string& pluginName,
                                     const std::string& json, int callbackId) {
    auto plugin = getPluginByName(pluginName);
    if (plugin != nullptr) {
        auto callback = plugin->pool_.at(callbackId);
        plugin->pool_.erase(callbackId);
        callback(json);
    }
}

void PluginManager::onNativeCallback(const std::string& pluginName,
                                     const std::string& json,
                                     const std::string& tag) {
    auto plugin = getPluginByName(pluginName);
    if (plugin != nullptr) {
        if (plugin->persistPool_.count(tag)) {
            plugin->persistPool_.at(tag)(json);
        }
    }
}

bool PluginManager::addPlugin(const std::string& pluginName,
                              PluginProtocol* plugin) {
    auto iter = plugins_.emplace(pluginName, plugin);
    return iter.second;
}

bool PluginManager::removePlugin(const std::string& pluginName) {
    if (plugins_.count(pluginName)) {
        plugins_.erase(pluginName);
        return true;
    }
    return false;
}
} // namespace core
} // namespace ee