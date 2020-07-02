//
//  PurchasingFactory.cpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#include "ee/store/private/StorePurchasingFactory.hpp"

#include "ee/store/StoreIPurchasingModule.hpp"
#include "ee/store/StoreIStore.hpp"

namespace ee {
namespace store {
using Self = PurchasingFactory;

std::shared_ptr<Self> Self::create(
    const std::shared_ptr<IPurchasingModule>& first,
    const std::vector<std::shared_ptr<IPurchasingModule>>& remainingModules) {
    auto result = std::shared_ptr<Self>(new Self());
    first->configure(result);
    for (auto&& remainingModule : remainingModules) {
        remainingModule->configure(result);
    }
    return result;
}

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

void Self::registerExtension(const std::string& key,
                             const std::shared_ptr<IStoreExtension>& instance) {
    extensionMap_[key] = instance;
}

void Self::registerConfiguration(
    const std::string& key,
    const std::shared_ptr<IStoreConfiguration>& instance) {
    configMap_[key] = instance;
}

std::shared_ptr<IStoreExtension>
Self::getExtension(const std::string& key) const {
    auto iter = extensionMap_.find(key);
    if (iter == extensionMap_.cend()) {
        return nullptr;
    }
    return iter->second;
}

std::shared_ptr<IStoreConfiguration>
Self::getConfig(const std::string& key) const {
    auto iter = configMap_.find(key);
    if (iter == configMap_.cend()) {
        return nullptr;
    }
    return iter->second;
}
} // namespace store
} // namespace ee
