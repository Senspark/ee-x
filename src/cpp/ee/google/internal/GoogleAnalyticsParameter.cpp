//
//  GoogleAnalyticsParameters.cpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#include "ee/google/internal/GoogleAnalyticsParameter.hpp"

namespace ee {
namespace google {
std::string make_parameter(const Parameter<0>& parameter) {
    return std::string("&") + parameter.get<0>();
}

std::string make_parameter(const Parameter<1>& parameter, std::size_t index) {
    return std::string("&") + parameter.get<0>() + std::to_string(index);
}

std::string make_parameter(const Parameter<1, 2>& parameter,
                           std::size_t index) {
    return std::string("&") + parameter.get<0>() + std::to_string(index) +
           parameter.get<1>();
}

std::string make_parameter(const Parameter<2>& parameter, std::size_t index0,
                           std::size_t index1) {
    return std::string("&") + parameter.get<0>() + std::to_string(index0) +
           parameter.get<1>() + std::to_string(index1);
}
} // namespace google
} // namespace ee
