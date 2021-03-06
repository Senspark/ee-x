//
//  StoreProductCollection.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_PRODUCT_COLLECTION_HPP
#define EE_X_STORE_PRODUCT_COLLECTION_HPP

#ifdef __cplusplus

#include <map>
#include <set>
#include <string>
#include <vector>

#include "ee/store/StoreFwd.hpp"

namespace ee {
namespace store {
class ProductCollection {
public:
    const std::set<std::shared_ptr<Product>>& set() const;
    const std::vector<std::shared_ptr<Product>>& all() const;
    std::shared_ptr<Product> withId(const std::string& id) const;
    std::shared_ptr<Product> withStoreSpecificId(const std::string& id) const;

private:
    friend PurchasingManager;

    template <class T>
    explicit ProductCollection(const T& products) {
        addProducts(products);
    }

    template <class T>
    void addProducts(const T& products) {
        for (auto&& product : products) {
            addProduct(product);
        }
    }

    void addProduct(const std::shared_ptr<Product>& product);

    std::vector<std::shared_ptr<Product>> products_;
    std::set<std::shared_ptr<Product>> productSet_;
    std::map<std::string, std::shared_ptr<Product>> idToProduct_;
    std::map<std::string, std::shared_ptr<Product>> storeSpecificIdToProduct_;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_PRODUCT_COLLECTION_HPP */
