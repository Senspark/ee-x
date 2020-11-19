# Firebase Crashlytics
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:firebase-crashlytics:1.3.0'
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
LOCAL_STATIC_LIBRARIES += ee_x_firebase_crashlytics
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
target_link_libraries(${PROJECT_NAME} ee_x_firebase_crashlytics)
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/firebase-crashlytics'
```

## Basic usage
Initialization
```cpp
#include <ee/Firebase.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IFirebaseCrashlytics>();
```

Logs a message
```cpp
plugin->log("message");
```