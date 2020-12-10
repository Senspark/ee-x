//
//  GoogleAnalyticsScreenViewBuilder.hpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#ifndef EE_X_GOOGLE_ANALYTICS_SCREEN_VIEW_BUILDER_HPP
#define EE_X_GOOGLE_ANALYTICS_SCREEN_VIEW_BUILDER_HPP

#ifdef __cplusplus

#include "ee/google/GoogleAnalyticsBuilder.hpp"

namespace ee {
namespace google {
/// https://developers.google.com/android/reference/com/google/android/gms/analytics/HitBuilders.ScreenViewBuilder
/// Class to build a screen view hit. You can add any of the other fields to the
/// builder using common set and get methods.
class ScreenViewBuilder : public Builder<ScreenViewBuilder> {
public:
    ScreenViewBuilder();
};
} // namespace google
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_GOOGLE_ANALYTICS_SCREEN_VIEW_BUILDER_HPP */
