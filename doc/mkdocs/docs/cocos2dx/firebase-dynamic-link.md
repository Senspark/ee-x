# Firebase Dynamic Link
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:firebase-dynamic-link:1.3.2'
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
LOCAL_STATIC_LIBRARIES += ee_x_firebase_dynamic_link
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
target_link_libraries(${PROJECT_NAME} ee_x_firebase_dynamic_link)
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/firebase-dynamic-link'
```

## Basic usage
Initialization
```cpp
#include <ee/Firebase.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IFirebaseDynamicLink>();
plugin->initialize();
plugin->setLinkReceivedCallback([](const std::string& link) {
    // Handle the received link.
});
plugin->fetch();
```