# ironSource
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:iron-source:1.0.0'

    // Includes:
    // - AdMob mediation.
    // - AppLovin mediation.
    // - FacebookAds mediation.
    // - Unity mediation.
    // - Vungle mediation.
    implementation 'com.senspark.ee:iron-source-mediation:1.0.0'
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
LOCAL_STATIC_LIBRARIES += ee_x_iron_source
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
target_link_libraries(${PROJECT_NAME} ee_x_iron_source)
```

### iOS
Modify `Podfile`
```ruby
pod ee-x/iron-source
```

## Basic usage
Initializes with game ID
```cpp
#include <ee/IronSource.hpp>

auto plugin = std::make_shared<ee::IronSource>();
plugin->initialize("your_game_id")
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