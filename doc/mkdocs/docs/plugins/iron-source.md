# ironSource
## Configuration
### Android
Modify `settings.gradle`
```java
include ':ee-x-ads'
include ':ee-x-iron-source'
project(':ee-x-ads').projectDir = new File('PATH/ee-x/proj.android_studio/ee-x-ads')
project(':ee-x-iron-source').projectDir = new File('PATH/ee-x/proj.android_studio/ee-x-iron-source')
```

Modify `build.gradle`
```java
dependencies {
    implementation project(':ee-x-iron-source')
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
pod ee-x/iron-source, :git => 'https://github.com/Senspark/ee-x'
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