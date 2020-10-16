# Adjust
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:adjust:1.1.1'
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
LOCAL_STATIC_LIBRARIES += ee_x_adjust
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
target_link_libraries(${PROJECT_NAME} ee_x_adjust)
```
### iOS
Modify `Podfile`
```ruby
pod 'ee-x/adjust'
pod 'Adjust', :modular_headers => true
```

## Basic usage
Initialization
```cpp
#include <ee/Adjust.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IAdjust>();
plugin->initialize(ee::AdjustConfig()
    .setToken("your_app_token")
    .setEnvironment(ee::AdjustEnvironment::Production));
```

Set push token
```cpp
plugin->setPushToken("your_push_token");
```