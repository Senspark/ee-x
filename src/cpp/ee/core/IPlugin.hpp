//
//  IPlugin.hpp
//  Pods
//
//  Created by eps on 6/15/20.
//

#ifndef EE_X_I_PLUGIN_HPP
#define EE_X_I_PLUGIN_HPP

#ifdef __cplusplus

#include "ee/CoreFwd.hpp"

namespace ee {
namespace core {
class IPlugin {
public:
    virtual ~IPlugin() = default;

    /// Destroys this plugin.
    virtual void destroy() = 0;
};
} // namespace core
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_I_PLUGIN_HPP */
