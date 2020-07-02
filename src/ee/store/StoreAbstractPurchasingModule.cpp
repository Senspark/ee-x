//
//  AbstractPurchasingModule.cpp
//  Pods
//
//  Created by eps on 7/2/20.
//

#include "ee/store/StoreAbstractPurchasingModule.hpp"

#include "ee/store/StoreIPurchasingBinder.hpp"

namespace ee {
namespace store {
using Self = AbstractPurchasingModule;

void Self::configure(const std::shared_ptr<IPurchasingBinder>& binder) {
    binder_ = binder;
    configure();
}

void Self::registerStore(const std::string& name,
                         const std::shared_ptr<IStore>& store) {
    binder_->registerStore(name, store);
}
} // namespace store
} // namespace ee
