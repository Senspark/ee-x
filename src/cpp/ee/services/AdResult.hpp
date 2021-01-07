//
//  AdResult.hpp
//  ee-x-d542b565
//
//  Created by eps on 1/7/21.
//

#ifndef EE_X_AD_RESULT_HPP
#define EE_X_AD_RESULT_HPP

#ifdef __cplusplus

#include "ee/services/ServicesFwd.hpp"

namespace ee {
namespace services {
enum class AdResult {
    NotInitialized,
    NotConfigured,
    Capped,
    NoInternet,
    NotLoaded,
    Failed,
    Canceled,
    Completed,
};
} // namespace services
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_AD_RESULT_HPP */
