## 2.10.3 - 2021-08-19
- [Unity] Fix creating AppsFlyer plugin.

## 2.10.2 - 2021-08-17
- [Unity] Add option to activate AppsFlyer.

## 2.10.1 - 2021-08-17
- [Android/iOS] Update AppsFlyer and Ads Libraries.
- [Unity] Add AppsFlyer.

## 2.10.0 - 2021-08-05
- [Android] Update method to get SHA1.
- [Android/iOS] Update ads libraries and Facebook SDK.

## 2.9.2 - 2021-07-05
- [Unity] Fix hanging when start ad.

## 2.9.1 - 2021-07-02
- [Android] Fix crash when load Unity Ads.
- [Unity] Fix long test connection when show ads.

## 2.9.0 - 2021-06-24
- [iOS] Add App Transparency Tracking.
- [Android/iOS] Update ads libraries.

## 2.8.1 - 2021-06-07
- [Unity] Fix build iOS with dynamic framework.

## 2.8.0 - 2021-06-04
- [Unity] Fix build iOS.
- [C++/Unity] Add auto-reload ad mechanism.

## 2.7.4 - 2021-05-21
- [Android] Fix crash with kotlin serialization library.
- [Android/iOS] Update ad and firebase libraries.

## 2.7.3 - 2021-05-07
- [Unity] Add option to fix package options related issues when building for Android.

## 2.7.2 - 2021-05-06
- [Android/iOS] Update ads libraries.

## 2.7.1 - 2021-04-14
- [Android] Update ads libraries.
- [Android] Separate AdMob test suite library.
- [iOS] Update Google Mobile Ads SDK from 7.x to 8.x.
- [Unity] Fix wrong parsing in GetDensity() method.
- [Unity] Fix info.plist not updated when build to iOS.
- [C++] Add addTestDevice method to IAdsManager.
- [Native] Not allow to display another full screen ad while displaying a full screen ad.

## 2.7.0 - 2021-04-10
- [Android] Upload libraries to maven central.
- [Unity] Add addTestDevice method to IAdsManager.
- [Unity] Add test rectangle banner ads.
- [Unity] Add option to parse default values from json file for FirebaseRemoteConfig.

## 2.6.4 - 2021-03-11
- [Android] Fix setDefaults in FirebaseRemoteConfig not work.

## 2.6.3 - 2021-03-04
- [Unity] Add Json (.net) serializer using reflection.
- [C++] Change initialize() method in IService.
- [Android] Fix initializing FirebaseRemoteConfig.

## 2.6.2 - 2021-03-01
- [Unity] Fix cannot compile on Unity 2020.1.

## 2.6.0 - 2021-03-01
- [Native] Fix crash in fetch() in Firebase Remote Config.
- [Android] Fix rewarded interstitial ad not registered correctly.
- [Android/iOS/Native] Add AdMob mediation test suite.
- [Unity] Add Audio, Analytics, RemoteConfig services.

## 2.5.0 - 2021-02-23
- [Android] Update Firebase BoM version to 26.5.0
- [Native] IAdsManager now has methods for each type of ad item.
- [JS] Fix to escape JSON message
- [C#] Auto inject SKAdNetworkItems list.

## 2.4.3 - 2021-02-15
- [Native] Handle invalid network / ad format in IAdsManager.
- [Native] getBool / getLong / getDouble / getString are now synchronized methods in Firebase Remote Config.
- [Native] Add Unity banner ad.

## 2.4.2 - 2021-02-11
- [iOS] Fix IronSource banner ad not displayed.
- [Native] Fix to reload banner ad in MultiBannerAd.

## 2.4.1 - 2021-02-05
- [C++] Remove Firebase C++ SDK.

## 2.4.0 - 2021-02-04
- [Android/iOS/Native] Add rewarded interstitial ads.
- [Android/iOS] Add TapJoy mediation adapters.
- [Android/iOS] Add Firebase Analytics / Remote Config.
- [iOS] Fix AppOpen ads freezing.
- [JS] Fix crash when initialize Soomla plugin.
- [JS] Fix ad callbacks not called.
- [JS] Remove Facebook JSB (use manual Facebook).

## 2.3.0 - 2021-01-25
- [Android/iOS/Native] Add IronSource banner ad.
- [JS] Remove legacy JSB libs (use manual libs):
  - AdMob.
  - Facebook Ads.
  - Firebase Crashlytics.
  - Firebase Performance.
  - IronSource.
  - Notification.
  - UnityAds.

## 2.2.3 - 2021-01-13
- [Native] Add capping when display consecutive full screen ads.

## 2.2.2 - 2021-01-09
- [C#] Fix crash when creating Facebook Ads interstitial ads.
- [C#] Fix rectangle banner ad not work in ads JSON config.

## 2.2.1 - 2021-01-08
- [Native] Add support for rectangle banner ad in ads JSON config.
- [Native] Add initialization time-out to ad network in ads JSON config.

## 2.2.0 - 2021-01-08
- [Native] Add loading cap for FAN ads.
- [C++] Add ads JSON config.

## 2.1.2 - 2021-01-05
- [Android] Fix UnityAds always reload.

## 2.1.1 - 2021-01-04
- [iOS] Lock RxSwift version.

## 2.1.0 - 2021-01-04
- [C#] Add ads JSON config.

## 2.0.6 - 2020-12-23
- [iOS] Fix build Unity.
- [iOS] Disable AdColony in AdMob mediation.

## 2.0.5 - 2020-12-21
- [Android] Fix to create ad only when load or show.

## 2.0.4 - 2020-12-18
- [C#] Fix build with Unity 2020.1.

## 2.0.3 - 2020-12-17
- [Android] Extract install referrer details.

## 2.0.2 - 2020-12-16
- [Android] Fix cannot create AdMob app open ads.

## 2.0.1 - 2020-12-16
- [C++] Fix compile

## 2.0.0 - 2020-12-16
- [Android/iOS/Native] Add AdMob app open ad.
- [Android/iOS/Native] Throw exception when initialize unmatched library versions.
- [C++] Use a single library dependency for cmake and ndk-build.

## 1.0.0 - 2020-08-04
- First release
- Supported plugins:
	 - AdMob (with AppLovin, FAN, IronSource, UnityAds, Vungle mediation).
	 - AppLovin.
	 - AppsFlyer.
	 - InstallReferrer.
	 - FAN.
	 - Facebook Login/Sharing.
	 - Firebase Analytics.
	 - Firebase Crashlytics.
	 - Firebase Dynamic Links.
	 - Firebase Messaging.
	 - Firebase Performance.
	 - Firebase Remote Config.
	 - Firebase Storage.
	 - Google Analytics.
	 - IronSource (with AdMob, AppLovin, FAN, UnityAds, Vungle mediation).
	 - Notification.
	 - Google Play Games / Apple Game Center.
	 - Soomla Store.
	 - Tenjin Analytics.
	 - Unity Ads.
	 - Vungle
