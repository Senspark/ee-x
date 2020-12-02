# AppsFlyer
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:apps-flyer:1.3.5'
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
LOCAL_STATIC_LIBRARIES += ee_x_apps_flyer
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
target_link_libraries(${PROJECT_NAME} ee_x_apps_flyer)
```
### iOS
Modify `Podfile`
```ruby
pod 'ee-x/apps-flyer'
```

## Basic usage
Initializes with dev key and app ID
```cpp
#include <ee/AppsFlyer.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IAppsFlyer>();
plugin->initialize("your_dev_key", "your_app_id");
```

Start tracking
```cpp
plugin->startTracking();
```