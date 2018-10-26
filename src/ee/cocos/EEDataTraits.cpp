//
//  EEDataTraits.cpp
//  ee-library
//
//  Created by Zinge on 7/26/16.
//
//

#include <sstream>
#include <stdexcept>

#include "EEDataTraits.hpp"

namespace ee {
template <> bool DataTraits<bool>::load(const std::string& value) {
    if (value == "1") {
        return true;
    }
    if (value == "0") {
        return false;
    }
    throw std::invalid_argument("DataTraits<bool>");
}

template <> int DataTraits<int>::load(const std::string& value) {
    return std::stoi(value);
}

template <> long DataTraits<long>::load(const std::string& value) {
    return std::stol(value);
}

template <> long long DataTraits<long long>::load(const std::string& value) {
    return std::stoll(value);
}

template <>
unsigned int DataTraits<unsigned int>::load(const std::string& value) {
    auto result = std::stoul(value);
    if (std::numeric_limits<unsigned int>::max() < result) {
        throw std::out_of_range("data_traits_unsigned_int");
    }
    return static_cast<unsigned int>(result);
}

template <>
unsigned long DataTraits<unsigned long>::load(const std::string& value) {
    return std::stoul(value);
}

template <>
unsigned long long
DataTraits<unsigned long long>::load(const std::string& value) {
    return std::stoull(value);
}

template <> float DataTraits<float>::load(const std::string& value) {
    return std::stof(value);
}

template <> double DataTraits<double>::load(const std::string& value) {
    return std::stod(value);
}

const std::string& DataTraits<std::string>::store(const std::string& value) {
    return value;
}

const std::string& DataTraits<std::string>::load(const std::string& value) {
    return value;
}
} // namespace ee
