//
//  GoogleAnalyticsEventBuilder.cpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#include "ee/google/GoogleAnalyticsEventBuilder.hpp"

#include "ee/google/internal/GoogleAnalyticsParameter.hpp"

namespace ee {
namespace google {
using Self = EventBuilder;

Self::EventBuilder() {
    setHitType(parameter::types::event);
}

Self::EventBuilder(const std::string& category, const std::string& action)
    : EventBuilder() {
    setCategory(category);
    setAction(action);
}

Self& Self::setCategory(const std::string& category) {
    return set(make_parameter(parameter::event_category), category);
}

Self& Self::setAction(const std::string& action) {
    return set(make_parameter(parameter::event_action), action);
}

Self& Self::setLabel(const std::string& label) {
    return set(make_parameter(parameter::event_label), label);
}

Self& Self::setValue(int value) {
    return set(make_parameter(parameter::event_value), std::to_string(value));
}
} // namespace google
} // namespace ee
