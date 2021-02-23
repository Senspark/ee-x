# AdMob
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:ad-mob-mediation:2.5.0'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/ad-mob-mediation', '2.5.0'
```

## Basic usage
Initialization
```cpp
#include <ee/Cpp.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IAdMob>();
auto result = co_await plugin->initialize();
// Handle initialization result.
```

Creates an show a banner ad
```cpp
auto ad = plugin->createBannerAd("ad_id", ee::AdMobBannerAdSize::Normal);

// Load the ad in background.
ee::noAwait(ad->load());

// Show the ad.
ad->setVisible(true);
```

Creates and show an interstitial ad
```cpp
auto ad = plugin->createInterstitialAd("ad_id");

// Load and show the ad.
ee::noAwait([ad]() -> ee::Task<> {
    co_await ad->load();
    co_await ad->show();
});
```

Creates and show a rewarded ad
```cpp
auto ad = plugin->createRewardedAd("ad_id");

// Load and show the ad.
ee::noAwait([ad]() -> ee::Task<> {
    co_await ad->load();
    co_await ad->show();
});
```