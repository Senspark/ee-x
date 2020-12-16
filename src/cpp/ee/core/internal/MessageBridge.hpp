//
//  BridgeManager.hpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#ifndef EE_X_CORE_MESSAGE_BRIDGE_HPP_
#define EE_X_CORE_MESSAGE_BRIDGE_HPP_

#ifdef __cplusplus

#include <unordered_map>

#include "ee/core/IMessageBridge.hpp"

namespace ee {
namespace core {
class MessageBridge final : public IMessageBridge {
private:
    using Self = MessageBridge;

public:
    static MessageBridge& getInstance();

    virtual bool registerHandler(const MessageHandler& handler,
                                 const std::string& tag) override;

    virtual bool deregisterHandler(const std::string& tag) override;

    virtual std::string call(const std::string& tag,
                             const std::string& message = "") override;

    [[nodiscard]] virtual Task<std::string>
    callAsync(const std::string& tag, const std::string& message = "") override;

    /// Calls a handler from C++ with a message.
    /// @warning This method should not be called manually.
    /// @param tag The unique tag of the handler.
    /// @param message The message.
    /// @return Reply message from C++.
    std::string callCpp(const std::string& tag, const std::string& message);

private:
    MessageBridge();
    ~MessageBridge();

    MessageBridge(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    MessageHandler findHandler(const std::string& tag) const;

    const Logger& logger_;

    int callbackCounter_;

    std::unique_ptr<SpinLock> handlerLock_;

    /// Registered handlers.
    std::unordered_map<std::string, MessageHandler> handlers_;
};
} // namespace core
} // namespace ee

#endif // __cplusplus

//#include "ee/core/Macro.hpp"

//#if defined(EE_X_IOS) || defined(EE_X_OSX)
//#ifdef __cplusplus
//extern "C" {
//#endif // __cplusplus
//
///// Required for Swift.
//
//
//#ifdef __cplusplus
//} // extern "C"
//#endif // __cplusplus
//#endif // defined(EE_X_IOS) || defined(EE_X_OSX)

#endif /* EE_X_CORE_MESSAGE_BRIDGE_HPP_ */
