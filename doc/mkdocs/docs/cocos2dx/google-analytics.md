# Google Analytics
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:google-analytics:1.0.0'
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
LOCAL_STATIC_LIBRARIES += ee_x_google_analytics
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
target_link_libraries(${PROJECT_NAME} ee_x_google_analytics)
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/google-analytics'
```

## Basic usage
Initialization
```cpp
#include <ee/GoogleAnalytics.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IGoogleAnalytics>();
```