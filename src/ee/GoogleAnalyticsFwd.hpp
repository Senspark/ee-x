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
using GoogleAnalyticsTracker = google::AnalyticsTracker;
using GoogleEventBuilder = google::EventBuilder;
using GoogleExceptionBuilder = google::ExceptionBuilder;
using GoogleScreenViewBuilder = google::ScreenViewBuilder;
using GoogleSocialBuilder = google::SocialBuilder;
using GoogleTimingBuilder = google::TimingBuilder;
using GoogleProduct = google::Product;
using GoogleProductAction = google::ProductAction;
} // namespace ee

#endif /* EE_X_GOOGLE_ANALYTICS_FWD_HPP */
