//
//  StoreIPurchasingModule.hpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#ifndef EE_X_STORE_I_PURCHASING_MODULE_HPP
#define EE_X_STORE_I_PURCHASING_MODULE_HPP

#ifdef __cplusplus

#include <memory>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class IPurchasingModule {
public:
    virtual void
    configure(const std::shared_ptr<IPurchasingBinder>& binder) = 0;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_I_PURCHASING_MODULE_HPP */
