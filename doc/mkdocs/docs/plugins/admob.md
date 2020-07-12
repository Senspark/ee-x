# AdMob
## Configuration
### Android
Modify `settings.gradle`
```java
include ':ee-x-ads'
include ':ee-x-admob'
project(':ee-x-ads').projectDir = new File('PATH/ee-x/proj.android_studio/ee-x-ads')
project(':ee-x-admob').projectDir = new File('PATH/ee-x/proj.android_studio/ee-x-admob')
```

Modify `build.gradle`
```java
dependencies {
    implementation project(':ee-x-admob')
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
pod ee-x/admob, :git => 'https://github.com/Senspark/ee-x'
```

## Basic usage
Initializes with application ID
```cpp
#include <ee/AdMob.hpp>

auto plugin = std::make_shared<ee::AdMob>();
plugin->initialize("your_app_id");
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