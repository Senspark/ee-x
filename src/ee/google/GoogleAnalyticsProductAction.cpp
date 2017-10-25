//
//  GoogleAnalyticsProductAction.cpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#include "ee/google/GoogleAnalyticsProductAction.hpp"
#include "ee/core/Utils.hpp"
#include "ee/google/internal/GoogleAnalyticsParameter.hpp"

namespace ee {
namespace google {
using Self = ProductAction;

const std::string Self::ActionAdd = "add";
const std::string Self::ActionCheckout = "checkout";
const std::string Self::ActionClick = "click";
const std::string Self::ActionDetail = "detail";
const std::string Self::ActionPurchase = "purchase";

Self::ProductAction(const std::string& action) {
    dict_[make_parameter(parameter::product_action)] = action;
}

Self& Self::setProductActionList(const std::string& value) {
    dict_[make_parameter(parameter::product_action_list)] = value;
    return *this;
}

Self& Self::setProductListSource(const std::string& value) {
    dict_[make_parameter(parameter::product_list_source)] = value;
    return *this;
}

Self& Self::setTransactionId(const std::string& value) {
    dict_[make_parameter(parameter::transaction_id)] = value;
    return *this;
}

Self& Self::setTransactionRevenue(float value) {
    dict_[make_parameter(parameter::transaction_revenue)] =
        core::toString(value);
    return *this;
}

TrackingDict Self::build() const {
    return dict_;
}
} // namespace google
} // namespace ee
