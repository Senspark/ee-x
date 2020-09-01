# Vungle
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:vungle:1.0.0'
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
LOCAL_STATIC_LIBRARIES += ee_x_vungle
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
target_link_libraries(${PROJECT_NAME} ee_x_vungle)
```

### iOS
Modify `Podfile`
```ruby
pod ee-x/vungle
```

## Basic usage
Initializes with game ID
```cpp
#include <ee/Vungle.hpp>

auto plugin = std::make_shared<ee::Vungle>();
plugin->initialize("your_game_id")
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