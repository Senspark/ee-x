//
//  StoreProductCollection.cpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#include "ee/store/StoreProductCollection.hpp"

#include "ee/store/StoreProduct.hpp"
#include "ee/store/StoreProductDefinition.hpp"

namespace ee {
namespace store {
using Self = ProductCollection;

void Self::addProduct(const std::shared_ptr<Product>& product) {
    if (productSet_.count(product)) {
        return;
    }
    productSet_.insert(product);
    products_.push_back(product);
    idToProduct_[product->definition()->id()] = product;
    storeSpecificIdToProduct_[product->definition()->storeSpecificId()] =
        product;
}

const std::set<std::shared_ptr<Product>>& Self::set() const {
    return productSet_;
}

const std::vector<std::shared_ptr<Product>>& Self::all() const {
    return products_;
}

std::shared_ptr<Product> Self::withId(const std::string& id) const {
    auto iter = idToProduct_.find(id);
    if (iter == idToProduct_.cend()) {
        return nullptr;
    }
    return iter->second;
}

std::shared_ptr<Product>
Self::withStoreSpecificId(const std::string& id) const {
    auto iter = storeSpecificIdToProduct_.find(id);
    if (iter == storeSpecificIdToProduct_.cend()) {
        return nullptr;
    }
    return iter->second;
}
} // namespace store
} // namespace ee
