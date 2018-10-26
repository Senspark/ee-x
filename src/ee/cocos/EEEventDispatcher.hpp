//
//  EEEventDispatcher.hpp
//  ee-library
//
//  Created by Zinge on 7/7/16.
//
//

#ifndef EE_LIBRARY_EVENT_DISPATCHER_HPP_
#define EE_LIBRARY_EVENT_DISPATCHER_HPP_

#include <memory>
#include <vector>

#include "EEEvent.hpp"
#include "EEEventInfo.hpp"
#include "EEEventListener.hpp"
#include "EEUtils.hpp"

namespace ee {
class EventDispatcher {
private:
    template <class T> struct is_event_info {
        static constexpr bool value =
            std::is_base_of<detail::EventInfoBase, T>::value;
    };

    template <class... Args> struct is_dispatchable {
        template <std::size_t Id, class... Ts>
        static constexpr bool value(EventInfo<Id, Ts...>) {
            return (sizeof...(Ts) == sizeof...(Args)) &&
                   (std::is_convertible<Args, Ts>::value && ...);
        }
    };

    template <class EventType, class Callable> struct can_be_dispatched {
        static constexpr bool value =
            std::is_convertible<Callable,
                                typename EventType::CallbackType>::value;
    };

public:
    EventDispatcher() = default;

    EventDispatcher(const EventDispatcher&) = delete;
    EventDispatcher(EventDispatcher&&) = default;

    EventDispatcher& operator=(const EventDispatcher&) = delete;
    EventDispatcher& operator=(EventDispatcher&&) = default;

    template <class EventType, class Callable>
    std::enable_if_t<is_event_info<EventType>::value &&
                     can_be_dispatched<EventType, Callable>::value>
    addListener(Callable&& callable) {
        addListener0(EventType{}, std::forward<Callable>(callable));
    }

    template <class EventType, class... Args>
    std::enable_if_t<is_event_info<EventType>::value &&
                     is_dispatchable<Args...>::value(EventType{})>
    dispatch(Args&&... args) {
        dispatch0(EventType{}, std::forward<Args>(args)...);
    }

    void clear() noexcept { listeners_.clear(); }

private:
    template <std::size_t Id, class... Args, class Callable>
    void addListener0(EventInfo<Id, Args...>, Callable&& callable) {
        using EventType = detail::Event<Args...>;
        using EventInfoType = EventInfo<Id, Args...>;
        using EventListenerType = detail::EventListener<Args...>;

        auto listener = EventListenerType::create(
            EventInfoType::getKey(),
            [=](EventType* event) { event->invoke(callable); });

        addListener1(listener);
        listeners_.push_back(ee::make_unique_listener(listener));
    }

    template <std::size_t Id, class... Ts, class... Args>
    void dispatch0(EventInfo<Id, Ts...>, Args&&... args) {
        using EventType = detail::Event<Ts...>;
        using EventInfoType = EventInfo<Id, Ts...>;

        EventType ev{EventInfoType::getKey()};
        ev.setData(std::forward<Args>(args)...);
        dispatch1(&ev);
    }

    void addListener1(cocos2d::EventListener* listener) const;
    void dispatch1(cocos2d::Event* event) const;

    std::vector<UniqueListener> listeners_;
};
} // namespace ee

#endif /* EE_LIBRARY_EVENT_DISPATCHER_HPP_ */
