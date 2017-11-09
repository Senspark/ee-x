#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/
cmake_minimum_required(VERSION 3.1)

include_directories(
  ${CMAKE_CURRENT_LIST_DIR}/src/ee
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/google
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/google/internal
)

set(GAME_SRC
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/GoogleAnalyticsBridge.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/GoogleAnalyticsBuilder.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/GoogleAnalyticsEventBuilder.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/GoogleAnalyticsExceptionBuilder.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/GoogleAnalyticsProduct.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/GoogleAnalyticsProductAction.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/GoogleAnalyticsScreenViewBuilder.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/GoogleAnalyticsSocialBuilder.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/GoogleAnalyticsTimingBuilder.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/GoogleAnalyticsTracker.cpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/internal/GoogleAnalyticsParameter.cpp
)

set(GAME_HEADERS_INCLUDE
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/GoogleAnalyticsBridge.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/GoogleAnalyticsBuilder.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/GoogleAnalyticsEventBuilder.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/GoogleAnalyticsExceptionBuilder.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/GoogleAnalyticsProduct.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/GoogleAnalyticsProductAction.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/GoogleAnalyticsScreenViewBuilder.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/GoogleAnalyticsSocialBuilder.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/GoogleAnalyticsTimingBuilder.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/GoogleAnalyticsTracker.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/google/internal/GoogleAnalyticsParameter.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/GoogleAnalytics.hpp
  ${CMAKE_CURRENT_LIST_DIR}/ee/GoogleAnalyticsFwd.hpp
)