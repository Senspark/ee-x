//
//  IMessageBridge.h
//  ee_x
//
//  Created by eps on 3/14/18.
//

#ifndef EE_X_CORE_I_MESSAGE_BRIDGE_HPP
#define EE_X_CORE_I_MESSAGE_BRIDGE_HPP

#include <functional>
#include <string>

#include "ee/CoreFwd.hpp"

namespace ee {
namespace core {
using MessageHandler = std::function<std::string(const std::string& message)>;

class IMessageBridge {
public:
    virtual ~IMessageBridge() = default;

    /// Calls a handler from other platforms without a message.
    /// @param tag The unique tag of the handler.
    /// @return Reply message from other platforms.
    virtual std::string call(const std::string& tag) = 0;

    /// Calls a handler from other platforms with a message.
    /// @param tag The unique tag of the handler.
    /// @param message The message.
    /// @return Reply message from other platforms.
    virtual std::string call(const std::string& tag,
                             const std::string& message) = 0;

    /// Calls a handler from C++ with a message.
    /// @warning This method should not be called manually.
    /// @param tag The unique tag of the handler.
    /// @param message The message.
    /// @return Reply message from C++.
    virtual std::string callCpp(const std::string& tag,
                                const std::string& message) = 0;

    /// Registers a handler to receive messages from other platforms.
    /// @param handler The handler to handle the message.
    /// @param tag The unique tag of the handler.
    /// @return Whether the registration was successful.
    virtual bool registerHandler(const MessageHandler& handler,
                                 const std::string& tag) = 0;

    /// Deregisters a handler not to receive messages from other platforms.
    /// @param tag The unique tag of the handler.
    /// @return Whether the deregistration was successful.
    virtual bool deregisterHandler(const std::string& tag) = 0;
};
} // namespace core
} // namespace ee

#endif /* EE_X_CORE_I_MESSAGE_BRIDGE_HPP */
