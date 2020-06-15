//
//  Factory.cpp
//  ee_x
//
//  Created by eps on 3/20/18.
//

#include "ee/core/PluginManager.hpp"

#include <string>
#include <unordered_map>

namespace ee {
namespace core {
using Self = PluginManager;

#if defined(EE_X_IOS) || defined(EE_X_OSX)
extern "C" {
void ee_staticInitializePlugins();
bool ee_staticAddPlugin(const std::string& name);
bool ee_staticRemovePlugin(const std::string& name);
} // extern "C"

void Self::initializePlugins() {
    ee_staticInitializePlugins();
}

namespace {
std::unordered_map<Plugin, std::string> pluginNames_ = {{
    {Plugin::Vungle, "Vungle"},
}};
} // namespace

bool Self::addPlugin(Plugin plugin) {
    auto&& name = pluginNames_.at(plugin);
    return ee_staticAddPlugin(name);
}

bool Self::removePlugin(Plugin plugin) {
    auto&& name = pluginNames_.at(plugin);
    return ee_staticRemovePlugin(name);
}
#endif // defined(EE_X_IOS) || defined(EE_X_OSX)
} // namespace core
} // namespace ee
