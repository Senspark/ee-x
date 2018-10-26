//
//  DynamicValue.h
//  roll-eat
//
//  Created by Hoang Le Hai on 7/8/15.
//
//

#ifndef EE_LIBRARY_DYNAMIC_VALUE_HPP_
#define EE_LIBRARY_DYNAMIC_VALUE_HPP_

#include <cstdint>
#include <memory>

namespace ee {
namespace detail {
template <class T, std::size_t Size = sizeof(T)> struct dynamic_value_storage;

template <class T> struct dynamic_value_storage<T, 4> {
    using value_type = T;
    using storage_type = std::uint32_t;
};

template <class T> struct dynamic_value_storage<T, 8> {
    using value_type = T;
    using storage_type = std::uint64_t;
};
} // namespace detail

/// Utility class to store a value that dynamically changes.
///
/// Used for anticheating.
template <class T> class DynamicValue final {
private:
    using Self = DynamicValue;

public:
    ~DynamicValue() = default;

    explicit DynamicValue(T value = T());

    DynamicValue(const Self& other);
    DynamicValue(Self&& other) = default;

    Self& operator=(const Self& other);
    Self& operator=(Self&& other) = default;

    /// Retrieves the value.
    T get() const;

    /// Assigns value.
    Self& set(T value);

    /// Add a value.
    Self& add(T amount);

    Self& subtract(T amount);

    /// Implicit conversion.
    operator T() const;

    Self& operator=(T value);
    Self& operator+=(T value);
    Self& operator-=(T value);

    /// Post-increment operator.
    Self operator++(int);

    /// Post-decrement operator.
    Self operator--(int);

    /// Pre-increment operator.
    Self& operator++();

    /// Pre-decrement operator.
    Self& operator--();

private:
    using storage_type =
        typename detail::dynamic_value_storage<T>::storage_type;

    std::unique_ptr<storage_type> value_;
    std::unique_ptr<storage_type> random_;
};
} // namespace ee

#endif /* EE_LIBRARY_DYNAMIC_VALUE_HPP_ */
