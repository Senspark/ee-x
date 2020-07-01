//
//  PurchasingFactory.cpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#include "ee/store/private/StorePurchasingFactory.hpp"

#include "ee/store/StoreIStore.hpp"

namespace ee {
namespace store {
using Self = PurchasingFactory;

const std::string& Self::storeName() const {
    return storeName_;
}

const std::shared_ptr<IStore>& Self::service() const {
    if (store_ != nullptr) {
        return store_;
    }
    throw std::runtime_error("No impl available!");
}

void Self::service(const std::shared_ptr<IStore>& store) {
    store_ = store;
}

void Self::registerStore(const std::string& name,
                         const std::shared_ptr<IStore>& store) {
    if (store_ != nullptr && store == nullptr) {
        return;
    }
    storeName_ = name;
    store_ = store;
}
} // namespace store
} // namespace ee
