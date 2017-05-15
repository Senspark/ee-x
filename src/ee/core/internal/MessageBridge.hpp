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

namespace ee {
namespace core {
using MessageHandler = std::function<std::string(const std::string& message)>;

class MessageBridge final {
public:
    static MessageBridge& getInstance();

    /// Sends a call to handlers from other platforms with no parameters.
    /// @param tag The handler's tag registered in other platforms.
    /// @return Reply message from native.
    std::string call(const std::string& tag);

    /// Sends a call to handlers from other platforms with the specified
    /// message.
    /// @param tag The handler's tag registered in other platforms.
    /// @param msg Additional parameter.
    /// @return Reply message from native.
    std::string call(const std::string& tag, const std::string& msg);

    /// Sends a call to registered handlers.
    /// @param tag The handler's tag registered.
    /// @param msg Additional parameter.
    /// @return Reply message.
    std::string callCpp(const std::string& tag, const std::string& msg);

    /// Registers a handler to receive message from other platforms.
    /// @param handler The handler to handle the message.
    /// @param tag The unique ID of the handler.
    /// @return Whether the registration was successful.
    bool registerHandler(const MessageHandler& handler, const std::string& tag);

    /// Deregisters a handler not to receive message from other platforms.
    /// @param tag The unique ID of the handler.
    /// @return Whether the deregistration was successful.
    bool deregisterHandlere(const std::string& tag);

private:
    MessageBridge();
    ~MessageBridge();

    MessageBridge(const MessageBridge&) = delete;
    MessageBridge& operator=(const MessageBridge&) = delete;

    std::map<std::string, MessageHandler> handlers_;
};
} // namespace core
} // namespace ee

#endif /* EE_X_CORE_MESSAGE_BRIDGE_HPP_ */
