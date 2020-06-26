//
//  StoreProduct.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_PRODUCT_HPP
#define EE_X_STORE_PRODUCT_HPP

#ifdef __cplusplus

#include <memory>
#include <string>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class Product {
private:
    using Self = Product;

public:
    explicit Product(const std::shared_ptr<ProductDefinition>& definition,
                     const std::shared_ptr<ProductMetadata>& metadata,
                     const std::string& receipt = "");

    Product(const Self&) = delete;
    Product& operator=(const Self&) = delete;

    Product(Self&&) = default;
    Product& operator=(Self&&) = default;

    const std::shared_ptr<ProductDefinition>& definition() const;
    const std::shared_ptr<ProductMetadata>& metadata() const;
    bool availableToPurchase() const;
    const std::string& transactionId() const;
    const std::string& receipt() const;

private:
    friend PurchasingManager;

    std::shared_ptr<ProductDefinition> definition_;
    std::shared_ptr<ProductMetadata> metadata_;
    bool availableToPurchase_;
    std::string transactionId_;
    std::string receipt_;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_PRODUCT_HPP */
