//
//  GoogleAnalyticsSocialBuilder.hpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#ifndef EE_X_GOOGLE_ANALYTICS_SOCIAL_BUILDER_HPP
#define EE_X_GOOGLE_ANALYTICS_SOCIAL_BUILDER_HPP

#ifdef __cplusplus

#include "ee/google/GoogleAnalyticsBuilder.hpp"

namespace ee {
namespace google {
/// https://developers.google.com/android/reference/com/google/android/gms/analytics/HitBuilders.SocialBuilder
/// A Builder object to build social event hits. See https://goo.gl/iydW9O for
/// description of all social fields.
class SocialBuilder : public Builder<SocialBuilder> {
private:
    using Self = SocialBuilder;

public:
    SocialBuilder();
    Self& setNetwork(const std::string& network);
    Self& setAction(const std::string& action);
    Self& setTarget(const std::string& target);
};
} // namespace google
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_GOOGLE_ANALYTICS_SOCIAL_BUILDER_HPP */
