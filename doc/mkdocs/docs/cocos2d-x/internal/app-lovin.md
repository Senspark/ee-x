# AppLovin
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:app-lovin:2.7.3'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/app-lovin', '2.7.3'
```

## Basic usage
Initializes with sdk key
```cpp
#include <ee/Cpp.hpp>

auto plugin = std::make_shared<ee::AppLovin>();
ee::noAwait([plugin]() -> ee::Task<> {
    auto result = co_await plugin->initialize("your_key");
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