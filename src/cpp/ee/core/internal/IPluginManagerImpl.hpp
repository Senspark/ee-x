#ifndef EE_X_I_PLUGIN_MANAGER_IMPL_HPP
#define EE_X_I_PLUGIN_MANAGER_IMPL_HPP

#include <string>

#include "ee/core/CoreFwd.hpp"

namespace ee {
namespace core {
class IPluginManagerImpl {
public:
    virtual ~IPluginManagerImpl() = default;
    virtual void initialize() = 0;
    virtual IMessageBridge& getBridge() = 0;
};
} // namespace core
} // namespace ee

#endif // EE_X_I_PLUGIN_MANAGER_IMPL_HPP