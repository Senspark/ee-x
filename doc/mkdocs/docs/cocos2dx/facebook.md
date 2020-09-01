# Facebook
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:facebook:1.0.0'
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
LOCAL_STATIC_LIBRARIES += ee_x_facebook
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
target_link_libraries(${PROJECT_NAME} ee_x_facebook)
```
### iOS
Modify `Podfile`
```ruby
pod ee-x/facebook
```

## Basic usage
Initialization
```cpp
#include <ee/Facebook.hpp>

auto plugin = ee::PluginManager::createPlugin<IFacebook>();
```