cmake_minimum_required(VERSION 3.1)

# project(ee_x_google_analytics)

include_directories(
  third_party/nlohmann/include
  src/ee/google/internal
  src/ee/google
)

set(EE_X_GOOGLE_SRC
  src/ee/google/GoogleAnalyticsBridge.cpp
  src/ee/google/GoogleAnalyticsBuilder.cpp
  src/ee/google/GoogleAnalyticsEventBuilder.cpp
  src/ee/google/GoogleAnalyticsExceptionBuilder.cpp
  src/ee/google/GoogleAnalyticsProduct.cpp
  src/ee/google/GoogleAnalyticsProductAction.cpp
  src/ee/google/GoogleAnalyticsScreenViewBuilder.cpp
  src/ee/google/GoogleAnalyticsSocialBuilder.cpp
  src/ee/google/GoogleAnalyticsTimingBuilder.cpp
  src/ee/google/GoogleAnalyticsTracker.cpp
  src/ee/google/internal/GoogleAnalyticsParameter.cpp
)

set(EE_X_GOOGLE_HEADERS
  src/ee/google/GoogleAnalyticsBridge.hpp
  src/ee/google/GoogleAnalyticsBuilder.hpp
  src/ee/google/GoogleAnalyticsEventBuilder.hpp
  src/ee/google/GoogleAnalyticsExceptionBuilder.hpp
  src/ee/google/GoogleAnalyticsProduct.hpp
  src/ee/google/GoogleAnalyticsProductAction.hpp
  src/ee/google/GoogleAnalyticsScreenViewBuilder.hpp
  src/ee/google/GoogleAnalyticsSocialBuilder.hpp
  src/ee/google/GoogleAnalyticsTimingBuilder.hpp
  src/ee/google/GoogleAnalyticsTracker.hpp
  src/ee/google/internal/GoogleAnalyticsParameter.hpp
)

# add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})

# target_link_libraries(${PROJECT_NAME}
#   ee_x_core
# )