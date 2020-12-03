# Campaign Receiver
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:campaign-receiver:1.3.6'
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
LOCAL_STATIC_LIBRARIES += ee_x_campaign_receiver
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
target_link_libraries(${PROJECT_NAME} ee_x_campaign_receiver)
```
### iOS
Modify `Podfile`
```ruby
pod 'ee-x/campaign-receiver'
```

## Basic usage
Initialization
```cpp
#include <ee/CampaignReceiver.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::ICampaignReceiver>();
plugin->initialize([](const std::string& link) {
    // Handle the received link.
});
```