# Firebase Remote Config
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:firebase-remote-config:2.2.2'
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
LOCAL_STATIC_LIBRARIES += ee_x_firebase_remote_config
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
target_link_libraries(${PROJECT_NAME} ee_x_firebase_remote_config)
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/firebase-remote-config', '2.2.2'
```

## Basic usage
Initialization
```cpp
#include <ee/Firebase.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IFirebaseRemoteConfig>();
plugin->initialize();
plugin->fetch(false, [](bool success) {
    // Handle fetch result.
});
```