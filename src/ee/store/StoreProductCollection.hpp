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
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class ProductCollection {
public:
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

    const std::vector<std::shared_ptr<Product>>& all() const;
    std::shared_ptr<Product> withId(const std::string& id);
    std::shared_ptr<Product> withStoreSpecificId(const std::string& id);

private:
    void addProduct(const std::shared_ptr<Product>& product);

    std::vector<std::shared_ptr<Product>> products_;
    std::map<std::string, Product*> idToProduct_;
    std::map<std::string, Product*> storeSpecificIdToProduct_;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_PRODUCT_COLLECTION_HPP */
