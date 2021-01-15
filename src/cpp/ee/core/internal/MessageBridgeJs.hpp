#ifndef EE_X_MESSAGE_BRIDGE_JS_HPP
#define EE_X_MESSAGE_BRIDGE_JS_HPP

#include "ee/core/IMessageBridge.hpp"

namespace ee {
namespace core {
class MessageBridgeJs final : public IMessageBridge {
public:
    MessageBridgeJs();
    virtual ~MessageBridgeJs() override;

    virtual void registerHandler(const MessageHandler& handler,
                                 const std::string& tag) override;

    virtual void deregisterHandler(const std::string& tag) override;

    virtual std::string call(const std::string& tag,
                             const std::string& message = "") override;

    [[nodiscard]] virtual Task<std::string>
    callAsync(const std::string& tag, const std::string& message = "") override;

    virtual void callCpp(const std::string& tag,
                         const std::string& message) override;
};
} // namespace core
} // namespace ee

#endif // EE_X_MESSAGE_BRIDGE_JS_HPP
