# Play
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:play:1.0.0'
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
LOCAL_STATIC_LIBRARIES += ee_x_play
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
target_link_libraries(${PROJECT_NAME} ee_x_play)
```

### iOS
Modify `Podfile`
```ruby
pod ee-x/play
```

## Basic usage
Initialization
```cpp
#include <ee/Play.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IPlay>();
```

Login / logout
```cpp
ee::noAwait([plugin]() -> ee::Task<> {
    auto result = co_await plugin->logIn();
    // Handle result.
});

ee::noAwait([plugin]() -> ee::Task<> {
    auto result = co_await plugin->logOut();
    // Handle result.
});
```