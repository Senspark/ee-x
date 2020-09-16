# Store
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:store:1.0.0'
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
LOCAL_STATIC_LIBRARIES += ee_x_store
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
target_link_libraries(${PROJECT_NAME} ee_x_store)
```

### iOS
Modify `Podfile`
```ruby
pod ee-x/store
```

## Basic usage
Initialization
```cpp
#include <ee/Store.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IStore>();
```