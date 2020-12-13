//
//  AdjustEnvironment.hpp
//  Adjust
//
//  Created by eps on 8/19/20.
//

#ifndef EE_X_ADJUST_ENVIRONMENT_HPP
#define EE_X_ADJUST_ENVIRONMENT_HPP

#ifdef __cplusplus

#include "ee/adjust/AdjustFwd.hpp"

namespace ee {
namespace adjust {
enum class Environment {
    Sandbox,
    Production,
};
} // namespace adjust
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_ADJUST_ENVIRONMENT_HPP */
