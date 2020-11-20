# AdMob
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:admob:1.3.1'

    // Includes:
    // - AppLovin mediation.
    // - FacebookAds mediation.
    // - IronSource mediation.
    // - Unity mediation.
    // - Vungle mediation.
    implementation 'com.senspark.ee:admob-mediation:1.3.1'
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
LOCAL_STATIC_LIBRARIES += ee_x_admob
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
target_link_libraries(${PROJECT_NAME} ee_x_admob)
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/admob', :git => 'https://github.com/Senspark/ee-x'
```

## Basic usage
Initialization
```cpp
#include <ee/AdMob.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IAdMob>();
ee::noAwait([plugin]() -> ee::Task<> {
    auto result = co_await plugin->initialize();
    // Handle initialization result.
});
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