# Firebase Analytics
## Configuration
### Android
Modify `settings.gradle`
```java
include ':ee-x-firebase-core'
include ':ee-x-firebase-analytics'
project(':ee-x-firebase-core').projectDir = new File('PATH/ee-x/proj.android_studio/ee-x-firebase-core')
project(':ee-x-firebase-analytics').projectDir = new File('PATH/ee-x/proj.android_studio/ee-x-firebase-analytics')
```

Modify `build.gradle`
```java
dependencies {
    implementation project(':ee-x-firebase-analytics')
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
pod ee-x/firebase-analytics, :git => 'https://github.com/Senspark/ee-x'
```

## Basic usage
Initialization
```cpp
#include <ee/Firebase.hpp>

auto plugin = std::make_shared<ee::FirebaseAnalytics>();
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