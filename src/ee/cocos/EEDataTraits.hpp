//
//  EEDataTraits.hpp
//  ee-library
//
//  Created by Zinge on 7/26/16.
//
//

#ifndef EE_DATA_TRAITS_HPP_
#define EE_DATA_TRAITS_HPP_

#include <chrono>
#include <string>
#include <type_traits>

#include "EEForward.hpp"
#include "EEDataMeta.hpp"

namespace ee {
/// Deserialize/serialize arithmetic types:
/// - bool
/// - int
/// - long
/// - long long
/// - unsigned int
/// - unsigned long
/// - unsigned long long
/// - float
/// - double
template <class T>
struct DataTraits<T, std::enable_if_t<std::is_arithmetic<T>::value>> {
    static std::string store(T value) { return std::to_string(value); };

    static T load(const std::string& value);
};

/// Deserialize/serialize string type.
template <> struct DataTraits<std::string> {
    static const std::string& store(const std::string& value);
    static const std::string& load(const std::string& value);
};

/// Deserialize/serialize enum types.
template <class T>
struct DataTraits<T, std::enable_if_t<std::is_enum<T>::value>> {
private:
    using U = std::underlying_type_t<T>;

public:
    static std::string store(T value) {
        return DataTraits<U>::store(static_cast<U>(value));
    }

    static T load(const std::string& value) {
        return static_cast<T>(DataTraits<U>::load(value));
    }
};

namespace detail {
template <class T> struct is_duration : std::false_type {};

template <class Rep, class Period>
struct is_duration<std::chrono::duration<Rep, Period>> : std::true_type {};

template <class T> constexpr bool is_duration_v = is_duration<T>::value;
} // namespace detail

/// Deserialize/serialize duration types.
template <class T>
struct DataTraits<T, std::enable_if_t<detail::is_duration_v<T>>> {
private:
    using Rep = typename T::rep;

public:
    template <class U, std::enable_if_t<detail::is_duration_v<U>, int> = 0>
    static std::string store(const U& value) {
        return DataTraits<Rep>::store(
            std::chrono::duration_cast<T>(value).count());
    }

    static T load(const std::string& value) {
        return T(DataTraits<Rep>::load(value));
    }
};
} // namespace ee

#endif /* EE_DATA_TRAITS_HPP_ */
