//
//  GoogleAnalyticsProductAction.hpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#ifndef EE_X_GOOGLE_ANALYTICS_PRODUCT_ACTION_HPP
#define EE_X_GOOGLE_ANALYTICS_PRODUCT_ACTION_HPP

#include "ee/GoogleAnalyticsFwd.hpp"

namespace ee {
namespace google {
/// https://developers.google.com/android/reference/com/google/android/gms/analytics/ecommerce/ProductAction
/// Class to construct transaction/checkout or other product interaction related
/// information for a Google Analytics hit. Use this class to report information
/// about products sold, viewed or refunded. This class is intended to be used
/// with Product. Instances of this class can be associated with @c
/// setProductAction(ProductAction).
class ProductAction {
private:
    using Self = ProductAction;

public:
    /// Action to use when a product is added to the cart.
    /// Constant Value: "add"
    static const std::string ActionAdd;

    /// Action to use for hits with checkout data. This action can have
    /// accompanying fields like checkout step @c setCheckoutStep(int), checkout
    /// label @c setCheckoutOptions(String) and checkout options
    /// @c setCheckoutOptions(String).
    /// Constant Value: "checkout"
    static const std::string ActionCheckout;

    /// Action to use when the user clicks on a set of products. This action can
    /// have accompanying fields like product action list name
    /// setProductActionList(String) and product list source
    /// setProductListSource(String).
    /// Constant Value: "click"
    static const std::string ActionClick;

    /// Action to use when the user views detailed descriptions of products.
    /// This action can have accompanying fields like product action list name
    /// setProductActionList(String) and product list source
    /// setProductListSource(String).
    /// Constant Value: "detail"
    static const std::string ActionDetail;

    /// Action that is used to report all the transaction data to GA. This is
    /// equivalent to the transaction hit type which was available in previous
    /// versions of the SDK. This action can can also have accompanying fields
    /// like transaction id, affiliation, revenue, tax, shipping and coupon
    /// code. These fields can be specified with methods defined in this class.
    /// Constant Value: "purchase"
    static const std::string ActionPurchase;

    /// Sets the product action for all the products included in the hit. Valid
    /// values include "detail", "click", "add", "remove", "checkout",
    /// "checkout_option", "purchase" and "refund". All these values are also
    /// defined in this class for ease of use. You also also send additional
    /// values with the hit for some specific actions. See the action
    /// documentation for details.
    /// @param action The value of product action.
    explicit ProductAction(const std::string& action);

    /// Sets the list name associated with the products in the analytics hit.
    /// This value is used with ACTION_DETAIL and ACTION_CLICK actions.
    /// @param value A string name identifying the product list.
    /// @return Returns the same object to enable chaining of methods.
    Self& setProductActionList(const std::string& value);

    /// Sets the list source name associated with the products in the analytics
    /// hit. This value is used with ACTION_DETAIL and ACTION_CLICK actions.
    /// @param value A string name identifying the product list's source.
    /// @return Returns the same object to enable chaining of methods.
    Self& setProductListSource(const std::string& value);

    /// The unique id associated with the transaction. This value is used for
    /// ACTION_PURCHASE and ACTION_REFUND actions.
    /// @param value A string id identifying a transaction.
    /// @return Returns the same object to enable chaining of methods.
    Self& setTransactionId(const std::string& value);

    /// Sets the transaction's total revenue. This value is used for
    /// ACTION_PURCHASE and ACTION_REFUND actions.
    /// @param value A number representing the revenue from a transaction.
    /// @return Returns the same object to enable chaining of methods.
    Self& setTransactionRevenue(float value);

    TrackingDict build() const;

private:
    TrackingDict dict_;
};
} // namespace google
} // namespace ee

#endif /* EE_X_GOOGLE_ANALYTICS_PRODUCT_ACTION_HPP */
