# Firebase Performance
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:firebase-performance:1.3.6'
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
LOCAL_STATIC_LIBRARIES += ee_x_firebase_performance
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
target_link_libraries(${PROJECT_NAME} ee_x_firebase_performance)
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/firebase-performance'
```

## Basic usage
Initialization
```cpp
#include <ee/Firebase.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IFirebasePerformance>();
auto trace = plugin->startTrace("default");
trace->start();
```