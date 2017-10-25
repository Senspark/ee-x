//
//  GoogleAnalyticsExceptionBuilder.cpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#include "ee/google/GoogleAnalyticsExceptionBuilder.hpp"
#include "ee/google/internal/GoogleAnalyticsParameter.hpp"

namespace ee {
namespace google {
using Self = ExceptionBuilder;

Self::ExceptionBuilder() {
    setHitType(parameter::types::exception);
}

Self& Self::setDescription(const std::string& description) {
    return set(make_parameter(parameter::exception_description), description);
}

Self& Self::setFatal(bool fatal) {
    return set(make_parameter(parameter::is_exception_fatal),
               std::to_string(fatal));
}
} // namespace google
} // namespace ee
