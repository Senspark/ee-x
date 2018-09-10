//
//  EEState.hpp
//  ee-library
//
//  Created by enrevol on 11/30/15.
//
//

#ifndef EE_LIBRARY_FLAGS_HPP_
#define EE_LIBRARY_FLAGS_HPP_

#include "EEMacro.hpp"

#define EE_ENABLE_BITWISE_OPERATORS_FOR_ENUM(EnumType)                         \
    namespace ee::detail {                                                     \
        template <>                                                            \
        struct has_bitwise_operators<EnumType> : public std::true_type {};     \
    } // namespace ee::detail

#define EE_ENABLE_ARITHMETIC_OPERATORS_FOR_ENUM(EnumType)                      \
    namespace ee::detail {                                                     \
        template <>                                                            \
        struct has_arithmetic_operators<EnumType> : public std::true_type {};  \
    } // namespace ee::detail

NS_EE_BEGIN
NS_DETAIL_BEGIN
template <class EnumType>
constexpr std::enable_if_t<std::is_enum<EnumType>::value,
                           std::underlying_type_t<EnumType>>
convert_enum_to_underlying_type(EnumType value) noexcept {
    return static_cast<std::underlying_type_t<EnumType>>(value);
}

template <class T> struct has_bitwise_operators : public std::false_type {};

template <class T> struct has_arithmetic_operators : public std::false_type {};

template <class EnumType, class T>
using enable_if_enum_has_bitwise_operators = std::enable_if_t<
    std::is_enum<EnumType>::value && has_bitwise_operators<EnumType>::value, T>;

template <class EnumType, class T>
using enable_if_enum_has_arithmetic_operators =
    std::enable_if_t<std::is_enum<EnumType>::value &&
                         has_arithmetic_operators<EnumType>::value,
                     T>;

template <class EnumType, class BinaryOp>
constexpr EnumType apply_binary_operator(EnumType lhs, EnumType rhs,
                                         BinaryOp op) noexcept {
    return static_cast<EnumType>(op(convert_enum_to_underlying_type(lhs),
                                    convert_enum_to_underlying_type(rhs)));
}

template <class EnumType, class UnaryOp>
constexpr EnumType apply_unary_operator(EnumType value, UnaryOp op) noexcept {
    return static_cast<EnumType>(op(convert_enum_to_underlying_type(value)));
}
NS_DETAIL_END
NS_EE_END

/// Bitwise OR operator.
template <class EnumType>
constexpr ee::detail::enable_if_enum_has_bitwise_operators<EnumType, EnumType>
operator|(EnumType lhs, EnumType rhs) noexcept {
    return ee::detail::apply_binary_operator(lhs, rhs, std::bit_or<>());
}

template <class EnumType>
constexpr ee::detail::enable_if_enum_has_bitwise_operators<EnumType, EnumType&>
operator|=(EnumType& lhs, EnumType rhs) noexcept {
    return lhs = lhs | rhs;
}

/// Bitwise AND operator.
template <class EnumType>
constexpr ee::detail::enable_if_enum_has_bitwise_operators<EnumType, EnumType>
operator&(EnumType lhs, EnumType rhs) noexcept {
    return ee::detail::apply_binary_operator(lhs, rhs, std::bit_and<>());
}

template <class EnumType>
constexpr ee::detail::enable_if_enum_has_bitwise_operators<EnumType, EnumType&>
operator&=(EnumType& lhs, EnumType rhs) noexcept {
    return lhs = lhs & rhs;
}

/// Bitwise XOR operator.
template <class EnumType>
constexpr ee::detail::enable_if_enum_has_bitwise_operators<EnumType, EnumType>
operator^(EnumType lhs, EnumType rhs) noexcept {
    return ee::detail::apply_binary_operator(lhs, rhs, std::bit_xor<>());
}

template <class EnumType>
constexpr ee::detail::enable_if_enum_has_bitwise_operators<EnumType, EnumType&>
operator^=(EnumType& lhs, EnumType rhs) noexcept {
    return lhs = lhs ^ rhs;
}

/// Bitwise NOT operator.
template <class EnumType>
constexpr ee::detail::enable_if_enum_has_bitwise_operators<EnumType, EnumType>
operator~(EnumType value) noexcept {
    return ee::detail::apply_unary_operator(value, std::bit_not<>());
}

/// Arithmetic addition operator.
template <class EnumType>
constexpr ee::detail::enable_if_enum_has_arithmetic_operators<EnumType,
                                                              EnumType>
operator+(EnumType lhs, std::underlying_type_t<EnumType> rhs) noexcept {
    return static_cast<EnumType>(
        ee::detail::convert_enum_to_underlying_type(lhs) + rhs);
}

template <class EnumType>
constexpr ee::detail::enable_if_enum_has_arithmetic_operators<EnumType,
                                                              EnumType&>
operator+=(EnumType& lhs, std::underlying_type_t<EnumType> rhs) noexcept {
    return lhs = lhs + rhs;
}

/// Arithmetic subtraction operator.
template <class EnumType>
constexpr ee::detail::enable_if_enum_has_arithmetic_operators<EnumType,
                                                              EnumType>
operator-(EnumType lhs, std::underlying_type_t<EnumType> rhs) noexcept {
    return static_cast<EnumType>(
        ee::detail::convert_enum_to_underlying_type(lhs) - rhs);
}

template <class EnumType>
constexpr ee::detail::enable_if_enum_has_arithmetic_operators<EnumType,
                                                              EnumType&>
operator-=(EnumType& lhs, std::underlying_type_t<EnumType> rhs) noexcept {
    return lhs = lhs - rhs;
}

/// Arithmetic pre-increment operator.
template <class EnumType>
constexpr ee::detail::enable_if_enum_has_arithmetic_operators<EnumType,
                                                              EnumType&>
operator++(EnumType& value) noexcept {
    return value += 1;
}

/// Arithmetic post-increment operator.
template <class EnumType>
constexpr ee::detail::enable_if_enum_has_arithmetic_operators<EnumType,
                                                              EnumType>
operator++(EnumType& value, int) noexcept {
    auto result = value;
    ++value;
    return result;
}

/// Arithmetic pre-decrement operator.
template <class EnumType>
constexpr ee::detail::enable_if_enum_has_arithmetic_operators<EnumType,
                                                              EnumType&>
operator--(EnumType& value) noexcept {
    return value -= 1;
}

/// Arithmetic post-decrement operator.
template <class EnumType>
constexpr ee::detail::enable_if_enum_has_arithmetic_operators<EnumType,
                                                              EnumType>
operator--(EnumType& value, int) noexcept {
    auto result = value;
    --value;
    return result;
}

#endif /* EE_LIBRARY_FLAGS_HPP_ */
