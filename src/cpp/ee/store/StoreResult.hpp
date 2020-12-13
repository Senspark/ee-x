//
//  StoreResult.hpp
//  Pods
//
//  Created by eps on 7/3/20.
//

#ifndef EE_X_STORE_RESULT_HPP
#define EE_X_STORE_RESULT_HPP

#ifdef __cplusplus

#include "ee/store/StoreFwd.hpp"

namespace ee {
namespace store {
enum class Result {
    True,
    False,
    Unsupported,
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_RESULT_HPP */
