# Unity Ads
## Configuration
### Android
Modify `settings.gradle`
```java
include ':ee-x-ads'
include ':ee-x-unity-ads'
project(':ee-x-ads').projectDir = new File('PATH/ee-x/proj.android_studio/ee-x-ads')
project(':ee-x-unity-ads').projectDir = new File('PATH/ee-x/proj.android_studio/ee-x-unity-ads')
```

Modify `build.gradle`
```java
dependencies {
    implementation project(':ee-x-unity-ads')
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
LOCAL_STATIC_LIBRARIES += ee_x_unity_ads
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
target_link_libraries(${PROJECT_NAME} ee_x_unity_ads)
```

### iOS
Modify `Podfile`
```ruby
pod ee-x/unity-ads, :git => 'https://github.com/Senspark/ee-x'
```

## Basic usage
Initializes with game ID
```cpp
#include <ee/UnityAds.hpp>

auto plugin = std::make_shared<ee::UnityAds>();
plugin->initialize("your_game_id", false)
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