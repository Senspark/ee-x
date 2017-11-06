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
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/internal
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/applovin
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/applovin/internal
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/core
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/internal
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/crashlytics
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/facebook
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/facebook/internal
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/firebase
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/ironsource
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/ironsource/internal
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/notification
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/store
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/internal
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/unityads
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/unityads/internal
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/vungle
  ${CMAKE_CURRENT_LIST_DIR}/src/ee/vungle/internal
  ${CMAKE_CURRENT_LIST_DIR}/third_party/nlohmann/include/ee/nlohmann
  ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/include/firebase
  ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/include/firebase/admob
  ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/include/firebase/analytics
  ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/include/firebase/auth
  ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/include/firebase/database
  ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/include/firebase/dynamic_links
  ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/include/firebase/internal
  ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/include/firebase/storage
  ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/include/firebase/
  ${CMAKE_CURRENT_LIST_DIR}/third_party/firebase_cpp_sdk/include/google_play_services
  ${CMAKE_CURRENT_LIST_DIR}/third_party/nlohmann/include
)

set(GAME_SRC
   ${CMAKE_CURRENT_LIST_DIR}/jni/cpp/ee/core/JniUtils.cpp
   ${CMAKE_CURRENT_LIST_DIR}/jni/cpp/ee/core/internal/JniMethodInfo.cpp
   ${CMAKE_CURRENT_LIST_DIR}/jni/cpp/ee/core/internal/JniString.cpp
   ${CMAKE_CURRENT_LIST_DIR}/jni/cpp/ee/core/internal/Logger_Android.cpp
   ${CMAKE_CURRENT_LIST_DIR}/jni/cpp/ee/core/internal/MessageBridge_Android.cpp
   ${CMAKE_CURRENT_LIST_DIR}/jni/cpp/ee/firebase/FirebaseApp.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/AdMobBridge.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/AdMobNativeAdLayout.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/internal/AdMobBannerAd.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/internal/AdMobInterstitialAd.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/internal/AdMobNativeAd.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/internal/AdMobRewardedVideo.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/AdViewInterface.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/InterstitialAdInterface.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/MultiAdView.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/MultiInterstitialAd.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/MultiRewardedVideo.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/RewardedVideoInterface.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/internal/AdViewBridgeHelper.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/internal/AdViewHelper.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/applovin/AppLovinBridge.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/applovin/internal/AppLovinRewardedVideo.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/Logger.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/LogLevel.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/Metrics.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/Utils.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/internal/MessageBridge.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/crashlytics/CrashlyticsBridge.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/facebook/FacebookAdsBridge.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/facebook/FacebookNativeAdLayout.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/facebook/internal/FacebookBannerAd.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/facebook/internal/FacebookInterstitialAd.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/facebook/internal/FacebookNativeAd.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/firebase/FirebaseAnalytics.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/firebase/FirebaseMessaging.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/firebase/FirebaseRemoteConfig.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/firebase/FirebaseStorage.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ironsource/IronSourceBridge.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ironsource/internal/IronSourceRewardedVideo.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/notification/NotificationBridge.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/notification/NotificationBuilder.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/KeyValueStorage.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/LifetimeVG.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/MarketItem.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/NativeKeyValueStorage.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/PurchasableVirtualItem.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/PurchaseType.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/PurchaseWithMarket.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/PurchaseWithVirtualItem.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/SingleUsePackVG.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/SingleUseVG.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/Store.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/StoreInfo.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/StoreLogger.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/StoreUtils.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/VirtualCurrency.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/VirtualCurrencyPack.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/VirtualCurrencyStorage.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/VirtualGood.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/VirtualGoodsStorage.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/VirtualItem.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/VirtualItemStorage.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/unityads/UnityAdsBridge.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/unityads/internal/UnityInterstitialAd.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/unityads/internal/UnityRewardedVideo.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/vungle/VungleBridge.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/vungle/internal/VungleRewardedVideo.cpp
)

set(GAME_HEADERS_INCLUDE
   ${CMAKE_CURRENT_LIST_DIR}/jni/cpp/ee/core/JniUtils.hpp
   ${CMAKE_CURRENT_LIST_DIR}/jni/cpp/ee/core/internal/JnimethodInfo.hpp
   ${CMAKE_CURRENT_LIST_DIR}/jni/cpp/ee/core/internal/JniString.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/AdMob.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/AdMobFwd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/Ads.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/AdsFwd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/AppLovin.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/AppLovinFwd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/Core.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/CoreFwd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/Crashlytics.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/CrashlyticsFwd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/Firebase.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/FirebaseFwd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/GoogleAnalytics.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/GoogleAnalyticsFwd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/IronSource.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/IronSourceFwd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/Macro.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/Notification.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/NotificationFwd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/UnityAds.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/UnityAdsFwd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/Vungle.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/VungleFwd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/AdMobBannerAdSize.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/AdMobBridge.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/AdMobNativeAdLayout.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/internal/AdMobBannerAd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/internal/AdMobInterstitialAd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/internal/AdMobNativeAd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/admob/internal/AdMobRewardedVideo.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/AdViewInterface.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/InterstitialAdInterface.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/MultiAdView.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/MultiInterstitialAd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/MultiRewardedVideo.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/NullAdView.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/NullInterstitialAd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/NullRewardedVideo.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/RewardedVideoInterface.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/internal/AdViewBridgeHelper.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/internal/AdViewHelper.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ads/internal/MediationManager.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/applovin/AppLovinBridge.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/applovin/internal/AppLovinRewardedVideo.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/Logger.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/LogLevel.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/Metrics.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/Utils.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/internal/MessageBridge.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/internal/ScopeGuard.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/core/internal/SpinLock.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/crashlytics/CrashlyticsBridge.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/facebook/FacebookAdsBridge.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/facebook/FacebookBannerAdSize.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/facebook/FacebookNativeAdLayout.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/facebook/internal/FacebookBannerAd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/facebook/internal/FacebookInterstitialAd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/facebook/internal/FacebookNativeAd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/firebase/FirebaseAnalytics.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/firebase/FirebaseApp.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/firebase/FirebaseMessaging.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/firebase/FirebaseRemoteConfig.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/firebase/FirebaseScheduler_inl.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/firebase/FirebaseScheduler.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/firebase/FirebaseStorage.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/google/GoogleAnalyticsBridge.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/google/GoogleAnalyticsBuilder.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/google/GoogleAnalyticsEventBuilder.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/google/GoogleAnalyticsExceptionBuilder.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/google/GoogleAnalyticsProduct.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/google/GoogleAnalyticsProductAction.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/google/GoogleAnalyticsScreenViewBuilder.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/google/GoogleAnalyticsSocialBuilder.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/google/GoogleAnalyticsTimingBuilder.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/google/GoogleAnalyticsTracker.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/google/internal/GoogleAnalyticsParameter.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ironsource/IronSourceBridge.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/ironsource/internal/IronSourceRewardedVideo.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/notification/NotificationBridge.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/notification/NotificationBuilder.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/KeyValueStorage.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/LifetimeVG.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/MarketItem.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/NativeKeyValueStorage.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/PurchasableVirtualItem.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/PurchaseType.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/PurchaseWithMarket.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/PurchaseWithVirtualItem.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/SingleUsePackVG.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/SingleUseVG.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/Store.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/StoreInfo.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/StoreLogger.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/StoreUtils.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/VirtualCurrency.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/VirtualCurrencyPack.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/VirtualCurrencyStorage.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/VirtualGood.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/VirtualGoodsStorage.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/VirtualItem.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/store/VirtualItemStorage.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/unityads/UnityAdsBridge.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/unityads/internal/UnityInterstitialAd.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/unityads/internal/UnityRewardedVideo.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/vungle/VungleBridge.hpp
   ${CMAKE_CURRENT_LIST_DIR}/src/ee/vungle/internal/VungleRewardedVideo.hpp
   ${CMAKE_CURRENT_LIST_DIR}/third_party/nlohmann/include/ee/nlohmann/json.hpp
)

add_library(ee-x STATIC ${GAME_SRC} ${GAME_HEADERS_INCLUDE})