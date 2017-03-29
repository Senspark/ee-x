//
//  PluginManager.hpp
//  ee_core
//
//  Created by Zinge on 3/29/17.
//
//

#ifndef EE_CORE_PLUGIN_MANAGER_HPP_
#define EE_CORE_PLUGIN_MANAGER_HPP_

#include <string>

namespace ee {
namespace core {
class PluginManager {
public:
    static PluginManager& getInstance();

    void addPlugin(const std::string& pluginName);

    void removePlugin(const std::string& pluginName);
};
} // namespace core
} // namespace ee

#endif /* EE_CORE_PLUGIN_MANAGER_HPP_ */
