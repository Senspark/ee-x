# Firebase Analytics
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:firebase-analytics:2.3.0'
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
LOCAL_STATIC_LIBRARIES += ee_x_firebase_analytics
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
target_link_libraries(${PROJECT_NAME} ee_x_firebase_analytics)
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/firebase-analytics', '2.3.0'
```

## Basic usage
Initialization
```cpp
#include <ee/Firebase.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IFirebaseAnalytics>();
```

Tracks current screen
```cpp
plugin->setCurrentScreen("current_screen");
```

Logs an event
```cpp
plugin->log("event_name", {
    {"key1", "value1"},
    {"key2", "value2"}
});
```