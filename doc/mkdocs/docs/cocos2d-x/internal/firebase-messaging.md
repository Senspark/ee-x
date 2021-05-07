# Firebase Messaging
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:firebase-messaging:2.7.3'
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
LOCAL_STATIC_LIBRARIES += ee_x_firebase_messaging
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
target_link_libraries(${PROJECT_NAME} ee_x_firebase_messaging)
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/firebase-messaging', '2.7.3'
```

## Basic usage
Initialization
```cpp
#include <ee/Firebase.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IFirebaseMessaging>();
plugin->initialize();
plugin->setMessageCallback([](const ee::Message& message) {
    // Handle the received message.
});
plugin->setTokenCallback([](const std::string& token) {
    // Handle the received token/
});
```