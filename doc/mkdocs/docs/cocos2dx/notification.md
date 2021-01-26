# Notification
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:notification:2.3.1'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/notification', '2.3.1'
```

## Basic usage
Initialization
```cpp
#include <ee/Cpp.hpp>

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