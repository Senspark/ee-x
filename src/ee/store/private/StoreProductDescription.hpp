//
//  StoreProductDescription.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_PRODUCT_DESCRIPTION_HPP
#define EE_X_STORE_PRODUCT_DESCRIPTION_HPP

#include <string>

#include "ee/store/StoreProductType.hpp"

namespace ee {
namespace store {
class ProductDescription {
private:
    using Self = ProductDescription;

public:
    ProductDescription(const std::string& id,
                       const std::shared_ptr<ProductMetadata>& metadata,
                       const std::string& receipt = "",
                       const std::string& transactionId = "",
                       ProductType type = ProductType::Consumable);

    ProductDescription(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    ProductDescription(Self&&) = default;
    Self& operator=(Self&&) = default;

    ProductType type() const;
    const std::string& storeSpecificId() const;
    const std::shared_ptr<ProductMetadata>& metadata() const;
    const std::string& receipt() const;
    const std::string& transactionId() const;

private:
    ProductType type_;
    std::string storeSpecificId_;
    std::shared_ptr<ProductMetadata> metadata_;
    std::string receipt_;
    std::string transactionId_;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_PRODUCT_DESCRIPTION_HPP */
