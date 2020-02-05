//
//  GoogleAnalyticsTimingBuilder.cpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#include "ee/google/GoogleAnalyticsTimingBuilder.hpp"

#include "ee/google/internal/GoogleAnalyticsParameter.hpp"

namespace ee {
namespace google {
using Self = TimingBuilder;

Self::TimingBuilder() {
    setHitType(parameter::types::timing);
}

Self::TimingBuilder(const std::string& category, const std::string& variable,
                    int value)
    : TimingBuilder() {
    setCategory(category);
    setVariable(variable);
    setValue(value);
}

Self& Self::setVariable(const std::string& variable) {
    return set(make_parameter(parameter::timing_variable_name), variable);
}

Self& Self::setValue(int value) {
    return set(make_parameter(parameter::timing_time), std::to_string(value));
}

Self& Self::setCategory(const std::string& category) {
    return set(make_parameter(parameter::timing_category), category);
}

Self& Self::setLabel(const std::string& label) {
    return set(make_parameter(parameter::timing_label), label);
}
} // namespace google
} // namespace ee
