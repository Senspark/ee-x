//
//  EEEventV2.hpp
//  ee-library
//
//  Created by Zinge on 7/7/16.
//
//

#ifndef EE_LIBRARY_EVENT_INFO_HPP
#define EE_LIBRARY_EVENT_INFO_HPP

#include <functional>
#include <string>
#include <tuple>

namespace ee {
namespace detail {
class EventInfoBase {};
} // namespace detail

template <std::size_t EventId, class... Args>
class EventInfo final : public detail::EventInfoBase {
public:
    using ArgTypes = std::tuple<Args...>;
    using CallbackType = std::function<void(Args...)>;

    enum { Id = EventId };

    static const std::string& getKey() {
        static const std::string key = "__EE_EVENT__" + std::to_string(Id);
        return key;
    }
};
} // namespace ee

#endif /* EE_LIBRARY_EVENT_INFO_HPP */
