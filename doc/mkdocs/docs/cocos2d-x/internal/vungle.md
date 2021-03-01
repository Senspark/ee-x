# Vungle
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:vungle:2.6.1'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/vungle', '2.6.1'
```

## Basic usage
Initializes with game ID
```cpp
#include <ee/Cpp.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IVungle>();
ee::noAwait([plugin]() -> ee::Task<> {
    auto result = co_await plugin->initialize("your_game_id");
    // Handle initialization result.
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