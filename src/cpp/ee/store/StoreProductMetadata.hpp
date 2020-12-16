//
//  ProductMetadata.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_PRODUCT_METADATA_HPP
#define EE_X_STORE_PRODUCT_METADATA_HPP

#ifdef __cplusplus

#include <string>

#include "ee/store/StoreFwd.hpp"

namespace ee {
namespace store {
class ProductMetadata {
private:
    using Self = ProductMetadata;

public:
    ProductMetadata();
    explicit ProductMetadata(const std::string& priceString,
                             const std::string& title,
                             const std::string& description,
                             const std::string& currencyCode,
                             double localizedPrice);

    ProductMetadata(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    ProductMetadata(Self&&) = default;
    Self& operator=(Self&&) = default;

    const std::string& localizedPriceString() const;
    const std::string& localizedTitle() const;
    const std::string& localizedDescription() const;
    const std::string& isoCurrencyCode() const;
    double localizedPrice() const;

private:
    std::string localizedPriceString_;
    std::string localizedTitle_;
    std::string localizedDescription_;
    std::string isoCurrencyCode_;
    double localizedPrice_;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_PRODUCT_METADATA_HPP */
