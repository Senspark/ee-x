//
//  GoogleAnalyticsEventBuilder.hpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#ifndef EE_X_GOOGLE_ANALYTICS_EVENT_BUILDER_HPP
#define EE_X_GOOGLE_ANALYTICS_EVENT_BUILDER_HPP

#include "ee/google/GoogleAnalyticsBuilder.hpp"

namespace ee {
namespace google {
/// https://developers.google.com/android/reference/com/google/android/gms/analytics/HitBuilders.EventBuilder
/// A Builder object to build event hits. For meaningful data, event hits
/// should contain at least the event category and the event action.
class EventBuilder : public Builder<EventBuilder> {
private:
    using Self = EventBuilder;

public:
    EventBuilder();

    /// Convenience constructor for creating an event hit. Additional fields can
    /// be specified using the setter methods.
    /// @param category in which the event will be filed. Example: "Video"
    /// @param action Action associated with the event. Example: "Play"
    explicit EventBuilder(const std::string& category,
                          const std::string& action);

    Self& setCategory(const std::string& category);
    Self& setAction(const std::string& action);
    Self& setLabel(const std::string& label);
    Self& setValue(int value);
};
} // namespace google
} // namespace ee

#endif /* EE_X_GOOGLE_ANALYTICS_EVENT_BUILDER_HPP */
