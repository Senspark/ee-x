//
//  GoogleAnalyticsFwd.hpp
//  ee_x
//
//  Created by Zinge on 10/25/17.
//
//

#ifndef EE_X_GOOGLE_ANALYTICS_FWD_HPP
#define EE_X_GOOGLE_ANALYTICS_FWD_HPP

#include <map>
#include <string>

namespace ee {
namespace google {
class Analytics;
class AnalyticsTracker;
class EventBuilder;
class ExceptionBuilder;
class ScreenViewBuilder;
class SocialBuilder;
class TimingBuilder;
class Product;
class ProductAction;
using TrackingDict = std::map<std::string, std::string>;
} // namespace google
using GoogleAnalytics = google::Analytics;
} // namespace ee

#endif /* EE_X_GOOGLE_ANALYTICS_FWD_HPP */
