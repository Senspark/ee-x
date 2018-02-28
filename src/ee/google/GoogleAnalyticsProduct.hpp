//
//  GoogleAnalyticsProduct.hpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#ifndef EE_X_GOOGLE_ANALYTICS_PRODUCT_HPP
#define EE_X_GOOGLE_ANALYTICS_PRODUCT_HPP

#include "ee/GoogleAnalyticsFwd.hpp"

namespace ee {
namespace google {
/// https://developers.google.com/android/reference/com/google/android/gms/analytics/ecommerce/Product
/// Class to construct product related information for a Google Analytics hit.
/// Use this class to report information about products sold by merchants or
/// impressions of products seen by users.
/// Instances of this class can be associated with both ProductActions via @c
/// addProduct(Product) and Product Impressions via @c addImpression(Product,
/// String).
class Product {
private:
    using Self = Product;

public:
    /// Sets the category associated with the product in GA reports.
    /// @param value The product's category. Example: "Toys"
    /// @return Returns the same object to enable chaining of methods.
    Self& setCategory(const std::string& value);

    /// Sets the id that is used to identify a product in GA reports.
    /// @param value The product id.
    /// @return Returns the same object to enable chaining of methods.
    Self& setId(const std::string& value);

    /// Sets the name that is used to identify the product in GA reports.
    /// @param value The product's name. Example: "Space Monkeys"
    /// @return Returns the same object to enable chaining of methods.
    Self& setName(const std::string& value);

    /// Sets the price of the product.
    /// @param price The product's price. Example: 3.14
    /// @return Returns the same object to enable chaining of methods.
    Self& setPrice(float price);

    /// Builds a Map of fields stored in this instance suitable for a product
    /// action.
    /// @param productIndex The index of this product in the product action
    /// list.
    TrackingDict build(std::size_t productIndex) const;

    /// Builds a Map of fields stored in this instance suitable for an
    /// impression list.
    /// @param listIndex The index of the product impression list.
    /// @param productIndex The index of this product in that impression list.
    TrackingDict build(std::size_t listIndex, std::size_t productIndex) const;

private:
    TrackingDict build(const std::string& prefix) const;

    TrackingDict dict_;
};
} // namespace google
} // namespace ee

#endif /* EE_X_GOOGLE_ANALYTICS_PRODUCT_HPP */
