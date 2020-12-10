//
//  GoogleAnalyticsSocialBuilder.cpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#include "ee/google/GoogleAnalyticsSocialBuilder.hpp"

#include "ee/google/internal/GoogleAnalyticsParameter.hpp"

namespace ee {
namespace google {
using Self = SocialBuilder;

Self::SocialBuilder() {
    setHitType(parameter::types::social);
}

Self& Self::setNetwork(const std::string& network) {
    return set(make_parameter(parameter::social_network), network);
}

Self& Self::setAction(const std::string& action) {
    return set(make_parameter(parameter::social_action), action);
}

Self& Self::setTarget(const std::string& target) {
    return set(make_parameter(parameter::social_action_target), target);
}
} // namespace google
} // namespace ee
