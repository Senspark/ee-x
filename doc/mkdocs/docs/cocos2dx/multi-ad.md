# Multi Ad
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:ad-mob-mediation:2.4.0'
    implementation 'com.senspark.ee:facebook-ads:2.4.0'
    implementation 'com.senspark.ee:iron-source-mediation:2.4.0'
    implementation 'com.senspark.ee:unity-ads:2.4.0'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/ad-mob-mediation', '2.4.0'
pod 'ee-x/facebook-ads', '2.4.0'
pod 'ee-x/iron-source-mediation', '2.4.0'
pod 'ee-x/unity-ads', '2.4.0'
```

## Basic usage
Initializes with JSON config
```cpp
auto config = cocos2d::FileUtils::getInstance()->getStringFromFile("config_path");
auto manager = std::make_shared<ee::DefaultAdsManager>(config);
ee::noAwait([manager]() -> ee::Task<> {
    co_await manager->initialize();
});
```

Normal banner ad
```cpp
// Show ad.
manager->setBannerAdVisible(true);

// Set ad position and anchor.
auto winSize = cocos2d::Director::getInstance()->getWinSize();
manager->setBannerAdPosition(winSize.width / 2, winSize.height / 2);
manager->setBannerAdAnchor(0.5f, 0.5f);
```

Rectangle banner ad
```cpp
// Show ad.
manager->setRectangleAdVisible(true);

// Set ad position and anchor.
auto winSize = cocos2d::Director::getInstance()->getWinSize();
manager->setRectangleAdPosition(winSize.width / 2, winSize.height / 2);
manager->setRectangleAdAnchor(0.5f, 0.5f);
```

App open ad
```cpp
auto result = co_await manager->showAppOpenAd();
if (result == ee::AdResult::Completed) {
    // Completed.
} else {
    // Failed.
}
```

Interstitial ad
```cpp
auto result = co_await manager->showInterstitialAd();
if (result == ee::AdResult::Completed) {
    // Completed.
} else {
    // Failed.
}
```

Rewarded interstitial ad
```cpp
auto result = co_await manager->showRewardedInterstitialAd();
if (result == ee::AdResult::Completed) {
    // Completed.
} else {
    // Failed.
}
```

Rewarded ad
```cpp
auto result = co_await manager->showRewardedAd();
if (result == ee::AdResult::Completed) {
    // Completed.
} else {
    // Failed.
}
```