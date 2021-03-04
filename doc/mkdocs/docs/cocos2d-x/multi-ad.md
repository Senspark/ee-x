# Multi Ad
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:ad-mob-mediation:2.6.3'
    implementation 'com.senspark.ee:facebook-ads:2.6.3'
    implementation 'com.senspark.ee:iron-source-mediation:2.6.3'
    implementation 'com.senspark.ee:unity-ads:2.6.3'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/ad-mob-mediation', '2.6.3'
pod 'ee-x/facebook-ads', '2.6.3'
pod 'ee-x/iron-source-mediation', '2.6.3'
pod 'ee-x/unity-ads', '2.6.3'
```

## Basic usage
Initializes with JSON config
```cpp
#include <ee/Cpp.hpp>

auto config = cocos2d::FileUtils::getInstance()->getStringFromFile("config_path");
auto manager = std::make_shared<ee::DefaultAdsManager>(config);
co_await manager->initialize();
```

Normal banner ad
```cpp
// Show ad.
manager->setBannerAdVisible(true);

// Set ad position and anchor.
auto winSize = cocos2d::Director::getInstance()->getWinSize();
manager->getBannerAd()->setPosition(winSize.width / 2, winSize.height / 2);
manager->getBannerAd()->setAnchor(0.5f, 0.5f);
```

Rectangle banner ad
```cpp
// Show ad.
manager->getRectangleAd()->setVisible(true);

// Set ad position and anchor.
auto winSize = cocos2d::Director::getInstance()->getWinSize();
manager->getRectangleAd()->setPosition(winSize.width / 2, winSize.height / 2);
manager->getRectangleAd()->setAnchor(0.5f, 0.5f);
```

App open ad
```cpp
auto result = co_await manager->getAppOpenAd()->show();
if (result == ee::AdResult::Completed) {
    // Completed.
} else {
    // Failed.
}
```

Interstitial ad
```cpp
auto result = co_await manager->getInterstitialAd()->show();
if (result == ee::AdResult::Completed) {
    // Completed.
} else {
    // Failed.
}
```

Rewarded interstitial ad
```cpp
auto result = co_await manager->getRewardedInterstitialAd()->show();
if (result == ee::AdResult::Completed) {
    // Completed.
} else {
    // Failed.
}
```

Rewarded ad
```cpp
auto result = co_await manager->getRewardedAd()->show();
if (result == ee::AdResult::Completed) {
    // Completed.
} else {
    // Failed.
}
```