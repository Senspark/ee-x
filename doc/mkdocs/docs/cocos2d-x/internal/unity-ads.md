# Unity Ads
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:unity-ads:2.6.0'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/unity-ads', '2.6.0'
```

## Basic usage
Initializes with game ID
```cpp
#include <ee/Cpp.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IUnityAds>();
ee::noAwait([plugin]() -> ee::Task<> {
    auto result = co_await plugin->initialize("your_game_id", false);
    // Handle initialization result.
});
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