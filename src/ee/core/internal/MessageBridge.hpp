//
//  BridgeManager.hpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#ifndef EE_X_CORE_MESSAGE_BRIDGE_HPP_
#define EE_X_CORE_MESSAGE_BRIDGE_HPP_

#include <map>

#include "ee/core/internal/IMessageBridge.hpp"

namespace ee {
namespace core {
class MessageBridge final : public IMessageBridge {
private:
    using Self = MessageBridge;

public:
    static MessageBridge& getInstance();

    /// @see Super.
    virtual std::string call(const std::string& tag) override;

    /// @see Super.
    virtual std::string call(const std::string& tag,
                             const std::string& message) override;

    /// @see Super.
    virtual std::string callCpp(const std::string& tag,
                                const std::string& message) override;

    /// @see Super.
    virtual bool registerHandler(const MessageHandler& handler,
                                 const std::string& tag) override;

    /// @see Super.
    virtual bool deregisterHandler(const std::string& tag) override;

private:
    MessageBridge();
    ~MessageBridge();

    MessageBridge(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    MessageHandler findHandler(const std::string& tag);

    std::unique_ptr<SpinLock> handlerLock_;

    /// Registered handlers.
    std::map<std::string, MessageHandler> handlers_;
};
} // namespace core
} // namespace ee

#endif /* EE_X_CORE_MESSAGE_BRIDGE_HPP_ */
