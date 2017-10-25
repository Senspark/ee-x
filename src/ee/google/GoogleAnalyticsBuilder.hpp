//
//  GoogleAnalyticsBuilder.hpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#ifndef EE_X_GOOGLE_ANALYTICS_BUILDER_HPP
#define EE_X_GOOGLE_ANALYTICS_BUILDER_HPP

#include <vector>

#include "ee/GoogleAnalyticsFwd.hpp"

namespace ee {
namespace google {
/// https://developers.google.com/android/reference/com/google/android/gms/analytics/HitBuilders.HitBuilder
/// Internal class to provide common builder class methods. The most important
/// methods from this class are the setXYZ and build methods. These methods can
/// be used to set individual properties on the hit and then build it so that it
/// is ready to be passed into the tracker.
template <class T>
class Builder {
private:
    using Self = T;

public:
    Builder();
    ~Builder();

    /// Adds a product impression to the hit. The product can be optionally
    /// associated with a named impression list.
    Self& addImpression(const Product& product,
                        const std::string& impressionList);

    /// Adds product information to be sent with a given hit. The action
    /// provided in @c setProductAction(ProductAction) affects how the products
    /// passed in through this method get processed.
    Self& addProduct(const Product& product);

    /// Sets a product action for all the products included in this hit. The
    /// action and its associated properties affect how the products added
    /// through @c addProduct(Product) are processed.
    Self& setProductAction(const ProductAction& action);

    /// Sets the value for the given parameter name.
    /// @param paramName The name of the parameter that should be sent over
    /// wire. This value should start with "&".
    /// @param paramValue The value to be sent over the wire for the given
    /// parameter.
    Self& set(const std::string& paramName, const std::string& paramValue);

    /// Adds a custom dimension to the current hit builder. Calling this method
    /// with the same index will overwrite the previous dimension with the new
    /// one.
    /// @param index The index/slot in which the dimension will be set.
    /// @param dimension The value of the dimension for the given index.
    Self& setCustomDimension(std::size_t index, const std::string& dimension);

    /// Adds a custom metric to the current hit builder. Calling this method
    /// with the same index will overwrite the previous metric with the new one.
    /// @param index The index/slot in which the dimension will be set.
    /// @param metric The value of the metric for the given index.
    Self& setCustomMetric(std::size_t index, float metric);

    /// Builds a Map of parameters and values that can be set on the Tracker
    /// object.
    /// @return A map of string keys to string values that can be passed into
    /// the tracker for one or more hits.
    TrackingDict build() const;

protected:
    /// Sets the type of the hit to be sent. This can be used to reuse the
    /// builder object for multiple hit types.
    /// @param hitType The value of the Hit.
    Self& setHitType(const std::string& hitType);

private:
    Self& self();

    TrackingDict dict_;
    std::vector<ProductAction> productAction_; ///< Actually contains 1 object.
    std::vector<Product> products_;
    std::map<std::string, std::vector<Product>> impressions_;
};
} // namespace google
} // namespace ee

#endif /* EE_X_GOOGLE_ANALYTICS_BUILDER_HPP */
