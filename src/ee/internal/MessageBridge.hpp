//
//  BridgeManager.hpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#ifndef EE_X_MESSAGE_BRIDGE_HPP_
#define EE_X_MESSAGE_BRIDGE_HPP_

#include <string>

namespace ee {
class MessageBridge final {
public:
    static MessageBridge& getInstance();

    /// Calls a method with no parameters
    std::string call(const std::string& tag);

    /// Calls a method with the specified message.
    std::string call(const std::string& tag, const std::string& msg);

private:
    MessageBridge();
    ~MessageBridge();
};
} // namespace ee

#endif /* EE_X_MESSAGE_BRIDGE_HPP_ */
