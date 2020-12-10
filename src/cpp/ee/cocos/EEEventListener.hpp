//
//  EEEventListener.hpp
//  ee-library
//
//  Created by enrevol on 3/23/16.
//
//

#ifndef EE_LIBRARY_EVENT_LISTENER_HPP_
#define EE_LIBRARY_EVENT_LISTENER_HPP_

#ifdef __cplusplus

#include <base/CCEventListener.h>

#include "ee/CocosFwd.hpp"

namespace ee {
namespace detail {
/// Variadic arguments (custom) event listener.
template <class... Args>
class EventListener final : public cocos2d::EventListener {
public:
    using EventType = Event<Args...>;
    using CallbackType = std::function<void(EventType*)>;

    /// Creates an event listener with name and callback.
    static EventListener* create(const std::string& eventName,
                                 const CallbackType& callback) {
        auto result = new (std::nothrow) EventListener();
        if (result != nullptr && result->init(eventName, callback)) {
            result->autorelease();
        } else {
            delete result;
            result = nullptr;
        }
        return result;
    }

    virtual bool checkAvailable() override {
        return cocos2d::EventListener::checkAvailable() && onCustomEvent_;
    }

    virtual EventListener* clone() override {
        return create(_listenerID, onCustomEvent_);
    }

private:
    bool init(const ListenerID& listenerId, const CallbackType& callback) {
        onCustomEvent_ = callback;
        auto listener = [this](cocos2d::Event* event) {
            if (onCustomEvent_) {
                onCustomEvent_(dynamic_cast<EventType*>(event));
            }
        };
        return cocos2d::EventListener::init(Type::CUSTOM, listenerId, listener);
    }

    CallbackType onCustomEvent_;
};
} // namespace detail
} // namespace ee

#endif // __cplusplus

#endif /* EE_LIBRARY_EVENT_LISTENER_HPP_ */
