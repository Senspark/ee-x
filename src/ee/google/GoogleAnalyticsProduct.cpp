//
//  GoogleAnalyticsProduct.cpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#include "ee/google/GoogleAnalyticsProduct.hpp"

#include <ee/core/Utils.hpp>

#include "ee/google/internal/GoogleAnalyticsParameter.hpp"

namespace ee {
namespace google {
using Self = Product;

namespace key {
constexpr auto category = "ca";
constexpr auto id = "id";
constexpr auto name = "nm";
constexpr auto price = "pr";
} // namespace key

Self& Self::setCategory(const std::string& value) {
    dict_[key::category] = value;
    return *this;
}

Self& Self::setId(const std::string& value) {
    dict_[key::id] = value;
    return *this;
}

Self& Self::setName(const std::string& value) {
    dict_[key::name] = value;
    return *this;
}

Self& Self::setPrice(float price) {
    dict_[key::price] = core::toString(price);
    return *this;
}

TrackingDict Self::build(std::size_t productIndex) const {
    return build(make_parameter(parameter::product_prefix, productIndex));
}

TrackingDict Product::build(std::size_t listIndex,
                            std::size_t productIndex) const {
    return build(make_parameter(parameter::product_impression_prefix, listIndex,
                                productIndex));
}

TrackingDict Product::build(const std::string& prefix) const {
    TrackingDict result;
    for (auto&& elt : dict_) {
        result[prefix + elt.first] = elt.second;
    }
    return result;
}
} // namespace google
} // namespace ee
