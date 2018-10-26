//
//  EEEvent.hpp
//  ee-library
//
//  Created by enrevol on 3/23/16.
//
//

#ifndef EE_LIBRARY_EVENT_HPP_
#define EE_LIBRARY_EVENT_HPP_

#include <functional>

#include "EEMacro.hpp"

#include <base/CCEventCustom.h>

NS_EE_BEGIN
NS_DETAIL_BEGIN
/// Variadic arguments (custom) event.
template <class... Args> class Event final : public cocos2d::EventCustom {
public:
    using ArgTypes = std::tuple<Args...>;
    using CallbackType = std::function<void(Args...)>;
    using InvokerType = std::function<void(const CallbackType& callback)>;

    using EventCustom::EventCustom;

    /// Assigns the event data to invoke later.
    template <class... Ts> void setData(Ts&&... args) {
        invoker_ = [&](const CallbackType& callback) {
            callback(std::forward<Ts>(args)...);
        };
    }

    /// Invokes the given callback with stored arguments.
    void invoke(const CallbackType& callback) const { invoker_(callback); }

private:
    InvokerType invoker_;
};
NS_DETAIL_END
NS_EE_END

#endif /* EE_LIBRARY_EVENT_HPP_ */
