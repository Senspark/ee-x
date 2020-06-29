//
//  StoreProductDefinition.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_PRODUCT_DEFINITION_HPP
#define EE_X_STORE_PRODUCT_DEFINITION_HPP

#ifdef __cplusplus

#include <string>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class ProductDefinition {
private:
    using Self = ProductDefinition;

public:
    ProductDefinition();
    explicit ProductDefinition(const std::string& id,
                               const std::string& storeSpecificId,
                               ProductType type);
    explicit ProductDefinition(const std::string& id, ProductType type);

    ProductDefinition(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    ProductDefinition(Self&&) = default;
    Self& operator=(Self&&) = default;

    const std::string& id() const;
    const std::string& storeSpecificId() const;
    ProductType type() const;

private:
    std::string id_;
    std::string storeSpecificId_;
    ProductType type_;
};

bool operator<(const ProductDefinition& lhs, const ProductDefinition& rhs);
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_PRODUCT_DEFINITION_HPP */
