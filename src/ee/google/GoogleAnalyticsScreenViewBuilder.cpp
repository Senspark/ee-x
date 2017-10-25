//
//  GoogleAnalyticsScreenViewBuilder.cpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#include "ee/google/GoogleAnalyticsScreenViewBuilder.hpp"
#include "ee/google/internal/GoogleAnalyticsParameter.hpp"

namespace ee {
namespace google {
using Self = ScreenViewBuilder;

Self::ScreenViewBuilder() {
    setHitType(parameter::types::screen_view);
}
} // namespace google
} // namespace ee
