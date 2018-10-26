//
//  EEDataMeta.hpp
//  ee-library
//
//  Created by Zinge on 7/21/17.
//
//

#ifndef EE_LIBRARY_DATA_META_HPP
#define EE_LIBRARY_DATA_META_HPP

#include <type_traits>

#include "EEForward.hpp"
#include "EEMeta.hpp"

namespace ee {
namespace detail {
template <class T> struct is_data_traits : std::false_type {};

template <class Value>
struct is_data_traits<DataTraits<Value, void>> : std::true_type {};

template <class T> constexpr bool is_data_traits_v = is_data_traits<T>::value;

/// Checks whether the specified traits type can store the specified value type
/// to std::string.
template <class Traits, class Value, class = void>
struct can_store : std::false_type {};

template <class Traits, class Value>
struct can_store<
    Traits, Value,
    std::enable_if_t<std::is_convertible<
        decltype(Traits::store(std::declval<Value>())), std::string>::value>>
    : std::true_type {};

template <class Traits, class Value>
constexpr bool can_store_v = can_store<Traits, Value>::value;

/// Checks whether the specified traits can load the specified value type from
/// std::string.
template <class Traits, class Value, class = void>
struct can_load : std::false_type {};

template <class Traits, class Value>
struct can_load<
    Traits, Value,
    std::enable_if_t<std::is_convertible<
        decltype(Traits::load(std::declval<std::string>())), Value>::value>>
    : std::true_type {};

template <class Traits, class Value>
constexpr bool can_load_v = can_load<Traits, Value>::value;

template <class Traits, class Value>
constexpr bool is_traits_v = (is_data_traits_v<Traits> ||
                              (can_store_v<Traits, Value> &&
                               can_load_v<Traits, Value>));

/// Checks whether the specified type is a DataInfo.
template <class T> struct is_data_info : std::false_type {};

template <std::size_t DataId, class Value, class... Keys>
struct is_data_info<DataInfo<DataId, Value, Keys...>> : std::true_type {};

template <class T> constexpr bool is_data_info_v = is_data_info<T>::value;

/// Checks whether it is possible to convert/construct from Args to Keys.
template <class Formatter, class... Args> struct is_formattable;

template <class... Keys, class... Args>
struct is_formattable<DataFormat<Keys...>, Args...> {
    static constexpr bool value = ((std::is_convertible<Args, Keys>::value ||
                                    std::is_constructible<Keys, Args>::value) &&
                                   ...);
};

template <class... Keys, class... Args>
struct is_formattable<DataFormat<std::tuple<Keys...>>, Args...>
    : is_formattable<DataFormat<Keys...>, Args...> {};

template <class Formatter, class... Args>
constexpr bool is_formattable_v = is_formattable<Formatter, Args...>::value;

/// Gets the last type in a parameter pack.
template <class... Args> struct last {
    using type =
        std::tuple_element_t<(sizeof...(Args)) - 1, std::tuple<Args...>>;
};

template <class... Args> using last_t = typename last<Args...>::type;

/// Gets all types except the last type in a parameter pack.
template <class... Args> struct exclude_last;

template <std::size_t... Is, class... Ts>
struct exclude_last<std::index_sequence<Is...>, Ts...> {
    using type =
        std::tuple<typename std::tuple_element<Is, std::tuple<Ts...>>::type...>;
};

template <class... Args>
using exclude_last_t =
    typename exclude_last<std::make_index_sequence<(sizeof...(Args)) - 1>,
                          Args...>::type;

template <class Value, class... Args> struct get_traits {
    using type = std::conditional_t<
        sizeof...(Args) == 0, DataTraits<Value>,
        std::conditional_t<is_traits_v<last_t<Args...>, Value>, last_t<Args...>,
                           DataTraits<Value>>>;
};

template <class Value, class... Args>
using get_traits_t = typename get_traits<Value, Args...>::type;

template <class Value, class... Args> struct get_format {
    using type = std::conditional_t<
        sizeof...(Args) == 0, DataFormat<>,
        std::conditional_t<is_traits_v<last_t<Args...>, Value>,
                           DataFormat<exclude_last_t<Args...>>,
                           DataFormat<Args...>>>;
};

template <class Value, class... Args>
using get_format_t = typename get_format<Value, Args...>::type;

template <class T, EE_REQUIRES(not std::is_enum<T>::value)>
const T& convert_enum_to_integer(const T& arg) {
    return arg;
}

template <class T, EE_REQUIRES(std::is_enum<T>::value)>
std::underlying_type_t<T> convert_enum_to_integer(const T& arg) {
    return static_cast<std::underlying_type_t<T>>(arg);
}
} // namespace detail
} // namespace ee

#endif /* EE_LIBRARY_DATA_META_HPP */
