# Notification
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:notification:1.3.2'
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
LOCAL_STATIC_LIBRARIES += ee_x_notification
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
target_link_libraries(${PROJECT_NAME} ee_x_notification)
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/notification'
```

## Basic usage
Initialization
```cpp
#include <ee/Notification.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::INotification>();
```

Schedule a notification
```cpp
plugin->schedule(ee::NotificationBuilder()
    .setTicker("ticker")
    .setTitle("title")
    .setBody("body")
    .setDelay(60)
    .setInternal(60)
    .setTag(0));
```