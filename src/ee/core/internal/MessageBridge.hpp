//
//  BridgeManager.hpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#ifndef EE_X_CORE_MESSAGE_BRIDGE_HPP_
#define EE_X_CORE_MESSAGE_BRIDGE_HPP_

#include <functional>
#include <map>
#include <string>

#include "ee/core/internal/SpinLock.hpp"

namespace ee {
namespace core {
using MessageHandler = std::function<std::string(const std::string& message)>;

class MessageBridge final {
private:
    using Self = MessageBridge;

public:
    static MessageBridge& getInstance();

    /// Calls a handler from other platforms without a message.
    /// @param tag The unique tag of the handler.
    /// @return Reply message from other platforms.
    std::string call(const std::string& tag);

    /// Calls a handler from other platforms with a message.
    /// @param tag The unique tag of the handler.
    /// @param message The message.
    /// @return Reply message from other platforms.
    std::string call(const std::string& tag, const std::string& message);

    /// Calls a handler from C++ with a message.
    /// @warning This method should not be called manually.
    /// @param tag The unique tag of the handler.
    /// @param message The message.
    /// @return Reply message from C++.
    std::string callCpp(const std::string& tag, const std::string& message);

    /// Registers a handler to receive messages from other platforms.
    /// @param handler The handler to handle the message.
    /// @param tag The unique tag of the handler.
    /// @return Whether the registration was successful.
    bool registerHandler(const MessageHandler& handler, const std::string& tag);

    /// Deregisters a handler not to receive messages from other platforms.
    /// @param tag The unique tag of the handler.
    /// @return Whether the deregistration was successful.
    bool deregisterHandler(const std::string& tag);

private:
    MessageBridge();
    ~MessageBridge();

    MessageBridge(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    MessageHandler findHandler(const std::string& tag);

    SpinLock handlerLock_;

    /// Registered handlers.
    std::map<std::string, MessageHandler> handlers_;
};
} // namespace core
} // namespace ee

#endif /* EE_X_CORE_MESSAGE_BRIDGE_HPP_ */
