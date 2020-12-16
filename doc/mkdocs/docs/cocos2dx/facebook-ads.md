# Facebook Ads
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:facebook-ads:2.0.0'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/facebook-ads'
```

## Basic usage
Initialization
```cpp
#include <ee/Cpp.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IFacebookAds>();
ee::noAwait([plugin]() -> ee::Task<> {
    auto result = co_await plugin->initialize();
    // Handle initialization result.
});
```

Creates and show a banner ad
```cpp
auto ad = plugin->createBannerAd("ad_id", ee::FacebookBannerAdSize::BannerHeight50);

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