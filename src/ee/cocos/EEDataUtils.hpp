//
//  EEDataUtils.hpp
//  ee-library
//
//  Created by Zinge on 7/20/17.
//
//

#ifndef EE_LIBRARY_DATA_UTILS_HPP
#define EE_LIBRARY_DATA_UTILS_HPP

#include <cstddef>
#include <string>

#include "EEDataMeta.hpp"

namespace ee {
namespace detail {
/// Globally sets a data value.
/// @param[in] dataId The data unique ID.
/// @param[in] key The data key.
/// @param[in] value The data value.
void set0(std::size_t dataId, const std::string& key, const std::string& value);

/// Globally gets a data value.
/// @param[in] dataId The data unique ID.
/// @param]in] key The data key.
/// @param[out] result The data value.
/// @return True if result is assigned, false otherwise.
bool get0(std::size_t dataId, const std::string& key, std::string& result);

/// Globally removes a data value.
/// @param[in] dataId The data unique ID.
/// @param[in] key The data key.
void remove0(std::size_t dataId, const std::string& key);
} // namespace detail

template <class DataType, class Traits = typename DataType::TraitsType,
          class Formatter = typename DataType::FormatType, class Value,
          class... Keys, EE_REQUIRES(detail::is_data_info_v<DataType>),
          EE_REQUIRES(detail::can_store_v<Traits, Value>),
          EE_REQUIRES(detail::is_formattable_v<Formatter, Keys...>)>
void set(Value&& value, Keys&&... keys) {
    detail::set0(DataType::Id,
                 Formatter::createKey(std::forward<Keys>(keys)...),
                 Traits::store(std::forward<Value>(value)));
}

template <class DataType, class Traits = typename DataType::TraitsType,
          class Formatter = typename DataType::FormatType,
          class Value = typename DataType::ValueType, class... Keys,
          EE_REQUIRES(detail::is_data_info_v<DataType>),
          EE_REQUIRES(detail::can_load_v<Traits, Value>),
          EE_REQUIRES(detail::is_formattable_v<Formatter, Keys...>)>
Value get(Keys&&... keys) {
    // Return type must not be decltype(auto) because
    // DataTraits<std::string>::load returns const std::string& would result a
    // segment fault.
    std::string result;
    detail::get0(DataType::Id,
                 Formatter::createKey(std::forward<Keys>(keys)...), result);
    return Traits::load(result);
}

template <class DataType, class Traits = typename DataType::TraitsType,
          class Formatter = typename DataType::FormatType,
          class Value = typename DataType::ValueType, class... Keys,
          EE_REQUIRES(detail::is_data_info_v<DataType>),
          EE_REQUIRES(detail::is_formattable_v<Formatter, Keys...>),
          EE_REQUIRES(detail::is_traits_v<Traits, Value>)>
void getAndSet(const typename DataType::SetterType& setter, Keys&&... keys) {
    auto current = get<DataType, Traits, Formatter>(keys...);
    setter(current);
    set<DataType, Traits, Formatter>(current, std::forward<Keys>(keys)...);
}

template <class DataType, class Traits = typename DataType::TraitsType,
          class Formatter = typename DataType::FormatType, class Function,
          class... Keys, class = std::enable_if_t<std::is_same<
                             bool, typename DataType::ValueType>::value>>
void getAndSetIf(bool conditionalValue, Function&& setter, Keys&&... keys) {
    auto current = get<DataType, Traits>(keys...);
    if (current == conditionalValue) {
        current = not conditionalValue;
        setter();
        set<DataType, Traits>(current, std::forward<Keys>(keys)...);
    }
}

template <class DataType, class... Keys> void remove(Keys&&... keys) {
    detail::remove0(DataType::Id,
                    DataType::createKey(std::forward<Keys>(keys)...));
}

class Increment final {
public:
    explicit Increment(std::size_t times = 1)
        : times_(times) {}

    template <class T> void operator()(T& value) const {
        for (std::size_t i = 0; i < times_; ++i) {
            ++value;
        }
    }

private:
    std::size_t times_;
};

class Decrement final {
public:
    explicit Decrement(std::size_t times = 1)
        : times_(times) {}

    template <class T> void operator()(T& value) const {
        for (std::size_t i = 0; i < times_; ++i) {
            --value;
        }
    }

private:
    std::size_t times_;
};
} // namespace ee

#endif /* EE_LIBRARY_DATA_UTILS_HPP */
