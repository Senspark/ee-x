//
//  StoreIStoreInternal.hpp
//  Pods
//
//  Created by eps on 7/2/20.
//

#ifndef EE_X_STORE_I_STORE_INTERNAL_HPP
#define EE_X_STORE_I_STORE_INTERNAL_HPP

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class IStoreInternal {
public:
    virtual void
    setModule(const std::shared_ptr<StandardPurchasingModule>& module) = 0;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_I_STORE_INTERNAL_HPP */
