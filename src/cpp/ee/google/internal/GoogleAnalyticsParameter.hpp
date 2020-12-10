//
//  GoogleAnalyticsParameters.hpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#ifndef EE_X_GOOGLE_ANALYTICS_PARAMETERS_HPP
#define EE_X_GOOGLE_ANALYTICS_PARAMETERS_HPP

#include <cstddef>
#include <string>
#include <type_traits>

namespace ee {
namespace google {
template <std::size_t Placeholders,
          std::size_t Parameters = (Placeholders > 1 ? Placeholders : 1)>
class Parameter {
public:
    template <class... Args, class = typename std::enable_if<sizeof...(Args) ==
                                                             Parameters>::type>
    constexpr explicit Parameter(Args&&... args)
        : s_{args...} {}

    template <std::size_t Index,
              class = typename std::enable_if<(Index < Parameters)>::type>
    constexpr const char* get() const {
        return s_[Index];
    }

private:
    const char* s_[Parameters];
};

namespace parameter {
/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#t
/// Required for all hit types.
/// The type of hit. Must be one of 'pageview', 'screenview', 'event',
/// 'transaction', 'item', 'social', 'exception', 'timing'.
constexpr auto hit_type = Parameter<0>("t");

namespace types {
constexpr auto event = "event";
constexpr auto screen_view = "screenview";
constexpr auto timing = "timing";
constexpr auto social = "social";
constexpr auto exception = "exception";
} // namespace types

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#cd
/// Required for screenview hit type.
constexpr auto screen_name = Parameter<0>("cd");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#ec
/// Required for event hit type.
/// Specifies the event category. Must not be empty.
constexpr auto event_category = Parameter<0>("ec");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#ea
/// Required for event hit type.
/// Specifies the event action. Must not be empty.
constexpr auto event_action = Parameter<0>("ea");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#el
/// Optional for event hit type.
/// Specifies the event label.
constexpr auto event_label = Parameter<0>("el");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#ev
/// Optional for event hit type.
/// Specifies the event value. Values must be non-negative.
constexpr auto event_value = Parameter<0>("ev");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#utc
/// Required for timing hit type.
/// Specifies the user timing category.
constexpr auto timing_category = Parameter<0>("utc");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#utv
/// Required for timing hit type.
/// Specifies the user timing variable.
constexpr auto timing_variable_name = Parameter<0>("utv");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#utt
/// Required for timing hit type.
/// Specifies the user timing value. The value is milliseconds.
constexpr auto timing_time = Parameter<0>("utt");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#utl
/// Optional for timing hit type.
/// Specifies the user timing label.
constexpr auto timing_label = Parameter<0>("utl");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#sn
/// Required for social hit type.
/// Specifies the social network, for example Facebook or Google Plus.
constexpr auto social_network = Parameter<0>("sn");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#sa
/// Required for social hit type.
/// Specifies the social interaction action. For example on Google Plus when a
/// user clicks the +1 button, the social action is 'plus'.
constexpr auto social_action = Parameter<0>("sa");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#st
/// Required for social hit type.
/// Specifies the target of a social interaction. This value is typically a URL
/// but can be any text.
constexpr auto social_action_target = Parameter<0>("st");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#exd
/// Optional for exception hit type.
/// Specifies the description of an exception.
constexpr auto exception_description = Parameter<0>("exd");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#exf
/// Optional for exception hit type.
/// Specifies whether the exception was fatal.
constexpr auto is_exception_fatal = Parameter<0>("exf");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#cd_
/// Optional for all hit types.
/// Each custom dimension has an associated index. There is a maximum of 20
/// custom dimensions (200 for Analytics 360 accounts). The dimension index must
/// be a positive integer between 1 and 200, inclusive.
constexpr auto custom_dimension = Parameter<1>("cd");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#cm_
/// Optional for all hit types.
/// Each custom metric has an associated index. There is a maximum of 20 custom
/// metrics (200 for Analytics 360 accounts). The metric index must be a
/// positive integer between 1 and 200, inclusive.
constexpr auto custom_metric = Parameter<1>("cm");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#pa
/// Optional for all hit types.
/// The role of the products included in a hit. If a product action is not
/// specified, all product definitions included with the hit will be ignored.
/// Must be one of: detail, click, add, remove, checkout, checkout_option,
/// purchase, refund. For analytics.js the Enhanced Ecommerce plugin must be
/// installed before using this field.
constexpr auto product_action = Parameter<0>("pa");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#ti
/// A unique identifier for the transaction. This value should be the same for
/// both the Transaction hit and Items hits associated to the particular
/// transaction.
constexpr auto transaction_id = Parameter<0>("ti");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#tr
/// Specifies the total revenue associated with the transaction. This value
/// should include any shipping or tax costs.
constexpr auto transaction_revenue = Parameter<0>("tr");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#pal
/// Optional for all hit types.
/// The list or collection from which a product action occurred. This is an
/// additional parameter that can be sent when Product Action is set to 'detail'
/// or 'click'. For analytics.js the Enhanced Ecommerce plugin must be installed
/// before using this field.
constexpr auto product_action_list = Parameter<0>("pal");

constexpr auto product_list_source = Parameter<0>("pls");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#pr_id
/// Product SKU.
/// Product Name.
/// Product Brand.
/// Product Category.
/// Product Variant.
/// Product Price.
/// Product Quantity.
/// Product Coupon Code.
/// Product Position.
/// Product Custom Dimension.
/// Product Custom Metric.
constexpr auto product_prefix = Parameter<1>("pr");

constexpr auto product_impression_prefix = Parameter<2>("il", "pi");

constexpr auto product_impression_list_name = Parameter<1, 2>("il", "nm");
} // namespace parameter

std::string make_parameter(const Parameter<0>& parameter);
std::string make_parameter(const Parameter<1>& parameter, std::size_t index);
std::string make_parameter(const Parameter<1, 2>& parameter, std::size_t index);
std::string make_parameter(const Parameter<2>& parameter, std::size_t index0,
                           std::size_t index1);
} // namespace google
} // namespace ee

#endif /* EE_X_GOOGLE_ANALYTICS_PARAMETERS_HPP */
