cmake_minimum_required(VERSION 3.1)

project(ee_x_google_analytics)

include_directories(
  ../../
  ../../../third_party/nlohmann/include
  internal
)

set(LIB_SRC
  GoogleAnalyticsBridge.cpp
  GoogleAnalyticsBuilder.cpp
  GoogleAnalyticsEventBuilder.cpp
  GoogleAnalyticsExceptionBuilder.cpp
  GoogleAnalyticsProduct.cpp
  GoogleAnalyticsProductAction.cpp
  GoogleAnalyticsScreenViewBuilder.cpp
  GoogleAnalyticsSocialBuilder.cpp
  GoogleAnalyticsTimingBuilder.cpp
  GoogleAnalyticsTracker.cpp
  internal/GoogleAnalyticsParameter.cpp
)

set(LIB_HEADERS
  GoogleAnalyticsBridge.hpp
  GoogleAnalyticsBuilder.hpp
  GoogleAnalyticsEventBuilder.hpp
  GoogleAnalyticsExceptionBuilder.hpp
  GoogleAnalyticsProduct.hpp
  GoogleAnalyticsProductAction.hpp
  GoogleAnalyticsScreenViewBuilder.hpp
  GoogleAnalyticsSocialBuilder.hpp
  GoogleAnalyticsTimingBuilder.hpp
  GoogleAnalyticsTracker.hpp
  internal/GoogleAnalyticsParameter.hpp
)

add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})

target_link_libraries(${PROJECT_NAME}
  ee_x_core
)