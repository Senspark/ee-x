//
//  EEDynamicValue.cpp
//  roll-eat
//
//  Created by enrevol on 9/2/15.
//
//

#include "EEDynamicValue.hpp"
#include "EEUtils.hpp"

#include <base/ccRandom.h>

namespace ee {
template <class T> using Self = DynamicValue<T>;

template <class T> Self<T>::DynamicValue(T value) {
    set(value);
}

template <class T>
Self<T>::DynamicValue(const Self& other)
    : DynamicValue() {
    set(other);
}

template <class T> Self<T>& Self<T>::operator=(const Self& other) {
    set(other);
    return *this;
}

template <class T> T Self<T>::get() const {
    auto intValue = (*value_) ^ (*random_);
    auto ret = bit_cast<T>(intValue);
    return ret;
}

template <class T> Self<T>& Self<T>::set(T value) {
    auto intValue = bit_cast<storage_type>(value);
    random_ = std::make_unique<storage_type>(
        cocos2d::random(std::numeric_limits<storage_type>::min(),
                        std::numeric_limits<storage_type>::max()));
    value_ = std::make_unique<storage_type>((*random_) ^ intValue);
    return *this;
}

template <class T> Self<T>& Self<T>::add(T amount) {
    return set(get() + amount);
}

template <class T> Self<T>& Self<T>::subtract(T amount) {
    return set(get() - amount);
}

template <class T> Self<T>::operator T() const {
    return get();
}

template <class T> Self<T>& Self<T>::operator=(T value) {
    return set(value);
}

template <class T> Self<T>& Self<T>::operator+=(T value) {
    return add(value);
}

template <class T> Self<T>& Self<T>::operator-=(T value) {
    return subtract(value);
}

template <class T> Self<T> Self<T>::operator++(int) {
    auto result = *this;
    add(1);
    return result;
}

template <class T> Self<T> Self<T>::operator--(int) {
    auto result = *this;
    subtract(1);
    return result;
}

template <class T> Self<T>& Self<T>::operator++() {
    return add(1);
}

template <class T> Self<T>& Self<T>::operator--() {
    return subtract(1);
}

template class DynamicValue<int>;
template class DynamicValue<long>;
template class DynamicValue<long long>;
template class DynamicValue<unsigned>;
template class DynamicValue<unsigned long>;
template class DynamicValue<unsigned long long>;
template class DynamicValue<float>;
template class DynamicValue<double>;
} // namespace ee
