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
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/internal
)

set(GAME_SRC
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/AdMobBridge.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/AdMobNativeAdLayout.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/internal/AdMobBannerAd.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/internal/AdMobInterstitialAd.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/internal/AdMobNativeAd.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/internal/AdMobRewardedVideo.cpp
)

set(GAME_HEADERS_INCLUDE
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/AdMob.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/AdMobFwd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/AdMobBannerAdSize.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/AdMobBridge.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/AdMobNativeAdLayout.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/internal/AdMobBannerAd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/internal/AdMobInterstitialAd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/internal/AdMobNativeAd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/internal/AdMobRewardedVideo.hpp
)