#ifndef EE_X_PLUGIN_MANAGER_IMPL_CPP_HPP
#define EE_X_PLUGIN_MANAGER_IMPL_CPP_HPP

#include "ee/core/internal/IPluginManagerImpl.hpp"

namespace ee {
namespace core {
class PluginManagerImplCpp : public IPluginManagerImpl {
public:
    explicit PluginManagerImplCpp(ILogger& logger);
    virtual void initialize() override;
    virtual IMessageBridge& getBridge() override;

private:
    ILogger& logger_;
    std::shared_ptr<IMessageBridge> bridge_;
};
} // namespace core
} // namespace ee

#endif // EE_X_PLUGIN_MANAGER_IMPL_CPP_HPP
