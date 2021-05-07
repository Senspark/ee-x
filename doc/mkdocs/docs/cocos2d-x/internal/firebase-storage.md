# Firebase Storage
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:firebase-storage:2.7.3'
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
LOCAL_STATIC_LIBRARIES += ee_x_firebase_storage
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
target_link_libraries(${PROJECT_NAME} ee_x_firebase_storage)
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/firebase-storage', '2.7.3'
```

## Basic usage
Initialization
```cpp
#include <ee/Firebase.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IFirebaseStorage>();
plugin->initialize();
```

Get file data
```cpp
plugin->getData("your_file_path", [](bool success, const std::string& data) {
    // Handle result.
});
```