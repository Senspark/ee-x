//
//  GoogleAnalyticsExceptionBuilder.hpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#ifndef EE_X_GOOGLE_ANALYTICS_EXCEPTION_BUILDER_HPP
#define EE_X_GOOGLE_ANALYTICS_EXCEPTION_BUILDER_HPP

#ifdef __cplusplus

#include "ee/google/GoogleAnalyticsBuilder.hpp"

namespace ee {
namespace google {
/// https://developers.google.com/android/reference/com/google/android/gms/analytics/HitBuilders.ExceptionBuilder
/// Exception builder allows you to measure the number and type of caught and
/// uncaught crashes and exceptions that occur in your app.
class ExceptionBuilder : public Builder<ExceptionBuilder> {
private:
    using Self = ExceptionBuilder;

public:
    ExceptionBuilder();
    Self& setDescription(const std::string& description);
    Self& setFatal(bool fatal);
};
} // namespace google
} // namespace ee

#endif // __cplusplus

#endif /* GoogleAnalyticsExceptionBuilder_hpp */
