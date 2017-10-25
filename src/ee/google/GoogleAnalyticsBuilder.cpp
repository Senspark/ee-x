//
//  GoogleAnalyticsBuilder.cpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#include <cassert>

#include "ee/core/Utils.hpp"
#include "ee/google/GoogleAnalyticsBuilder.hpp"
#include "ee/google/GoogleAnalyticsEventBuilder.hpp"
#include "ee/google/GoogleAnalyticsExceptionBuilder.hpp"
#include "ee/google/GoogleAnalyticsProduct.hpp"
#include "ee/google/GoogleAnalyticsProductAction.hpp"
#include "ee/google/GoogleAnalyticsScreenViewBuilder.hpp"
#include "ee/google/GoogleAnalyticsSocialBuilder.hpp"
#include "ee/google/GoogleAnalyticsTimingBuilder.hpp"
#include "ee/google/internal/GoogleAnalyticsParameter.hpp"

namespace ee {
namespace google {
namespace {
/// Merges two maps.
void merge(TrackingDict& lhs, const TrackingDict& rhs) {
    for (auto&& elt : rhs) {
        lhs[elt.first] = elt.second;
    }
}
} // namespace

template <class T>
using Self = Builder<T>;

template <class T>
Self<T>::Builder() {}

template <class T>
Self<T>::~Builder() {}

template <class T>
T& Self<T>::self() {
    return static_cast<Self&>(*this);
}

template <class T>
T& Self<T>::addImpression(const Product& product,
                          const std::string& impressionList) {
    impressions_[impressionList].push_back(product);
    return self();
}

template <class T>
T& Self<T>::addProduct(const Product& product) {
    products_.push_back(product);
    return self();
}

template <class T>
T& Self<T>::setProductAction(const ProductAction& action) {
    productAction_.clear();
    productAction_.push_back(action);
    return self();
}

template <class T>
T& Self<T>::set(const std::string& paramName, const std::string& paramValue) {
    dict_[paramName] = paramValue;
    return self();
}

template <class T>
T& Self<T>::setCustomDimension(std::size_t index,
                               const std::string& dimension) {
    return set(make_parameter(parameter::custom_dimension, index), dimension);
}

template <class T>
T& Self<T>::setCustomMetric(std::size_t index, float metric) {
    return set(make_parameter(parameter::custom_metric, index),
               core::toString(metric));
}

template <class T>
TrackingDict Self<T>::build() const {
    auto result = dict_;
    if (not products_.empty()) {
        std::size_t productIndex = 1;
        for (auto&& product : products_) {
            merge(result, product.build(productIndex));
            ++productIndex;
        }
    }
    if (not productAction_.empty()) {
        merge(result, productAction_[0].build());
    }
    if (not impressions_.empty()) {
        std::size_t listIndex = 1;
        for (auto&& elt : impressions_) {
            result[make_parameter(parameter::product_impression_list_name,
                                  listIndex)] = elt.first;
            std::size_t productIndex = 1;
            for (auto&& product : elt.second) {
                merge(result, product.build(listIndex, productIndex));
                ++productIndex;
            }
            ++listIndex;
        }
    }
    for (auto&& elt : result) {
        assert(elt.first[0] == '&');
    }
    return result;
}

template <class T>
T& Self<T>::setHitType(const std::string& hitType) {
    return set(make_parameter(parameter::hit_type), hitType);
}

template class Builder<EventBuilder>;
template class Builder<ExceptionBuilder>;
template class Builder<ScreenViewBuilder>;
template class Builder<SocialBuilder>;
template class Builder<TimingBuilder>;
} // namespace google
} // namespace ee
