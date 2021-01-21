#ifndef EE_X_PLUGIN_MANAGER_IMPL_JS_HPP
#define EE_X_PLUGIN_MANAGER_IMPL_JS_HPP

#include <ee/cocos/CocosFwd.hpp>

#ifdef EE_X_COCOS_JS

#include "ee/core/internal/IPluginManagerImpl.hpp"

namespace ee {
namespace core {
class PluginManagerImplJs : public IPluginManagerImpl {
public:
    virtual void initialize() override;
    virtual IMessageBridge& getBridge() override;

private:
    std::shared_ptr<IMessageBridge> bridge_;
};
} // namespace core
} // namespace ee

#endif // EE_X_COCOS_JS

#endif // EE_X_PLUGIN_MANAGER_IMPL_JS_HPP
