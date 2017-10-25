//
//  GoogleAnalyticsTimingBuilder.hpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#ifndef EE_X_GOOGLE_ANALYTICS_TIMING_BUILDER_HPP
#define EE_X_GOOGLE_ANALYTICS_TIMING_BUILDER_HPP

#include "ee/google/GoogleAnalyticsBuilder.hpp"

namespace ee {
namespace google {
/// https://developers.google.com/android/reference/com/google/android/gms/analytics/HitBuilders.TimingBuilder
/// Hit builder used to collect timing related data. For example, this hit type
/// can be useful to measure resource load times. For meaningful data, at least
/// the category and the value should be set before sending the hit.
class TimingBuilder : public Builder<TimingBuilder> {
private:
    using Self = TimingBuilder;

public:
    TimingBuilder();

    /// Convenience constructor for creating a timing hit. Additional fields can
    /// be specified using the setter methods.
    /// @param category The type of variable being measured. Example:
    /// AssetLoader
    /// @param variable The variable being measured. Example: AssetLoader.load
    /// @param value The value associated with the variable. Example: 1000
    explicit TimingBuilder(const std::string& category,
                           const std::string& variable, int value);

    Self& setVariable(const std::string& variable);
    Self& setValue(int value);
    Self& setCategory(const std::string& category);
    Self& setLabel(const std::string& label);
};
} // namespace google
} // namespace ee

#endif /* EE_X_GOOGLE_ANALYTICS_TIMING_BUILDER_HPP */
