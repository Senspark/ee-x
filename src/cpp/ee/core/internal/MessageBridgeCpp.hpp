//
//  BridgeManager.hpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#ifndef EE_X_CORE_MESSAGE_BRIDGE_CPP_HPP_
#define EE_X_CORE_MESSAGE_BRIDGE_CPP_HPP_

#include <unordered_map>

#include "ee/core/IMessageBridge.hpp"

namespace ee {
namespace core {
class MessageBridgeCpp final : public IMessageBridge {
public:
    MessageBridgeCpp();
    virtual ~MessageBridgeCpp() override;

    virtual void registerHandler(const MessageHandler& handler,
                                 const std::string& tag) override;

    virtual void deregisterHandler(const std::string& tag) override;

    virtual std::string call(const std::string& tag,
                             const std::string& message = "") override;

    [[nodiscard]] virtual Task<std::string>
    callAsync(const std::string& tag, const std::string& message = "") override;

    virtual void callCpp(const std::string& tag,
                         const std::string& message) override;

private:
    MessageHandler findHandler(const std::string& tag) const;

    const Logger& logger_;

    int callbackCounter_;

    /// Registered handlers.
    std::unordered_map<std::string, MessageHandler> handlers_;
};
} // namespace core
} // namespace ee

#endif /* EE_X_CORE_MESSAGE_BRIDGE_CPP_HPP */
