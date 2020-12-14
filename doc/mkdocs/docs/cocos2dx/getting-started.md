# Getting Started
## Configuration
### Android
Modify `build.gradle`
```java
repositories {
    maven { url 'https://dl.bintray.com/enrevol/ee-x' }
}

dependencies {
    implementation 'com.senspark.ee:core:2.0.0'
}
```

Clone the repository and let `PATH` be where you cloned the repository to
#### For `ndk-build` users
Modify `Android.mk`
```
$(call import-add-path, PATH)
$(call import-module, ee-x)
LOCAL_STATIC_LIBRARIES += ee_x
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
add_subdirectory(PATH/ee-x ${PROJECT_BINARY_DIR}/ee-x)
target_link_libraries(${PROJECT_NAME} ee_x)
```

### iOS
Install CocoaPods and modify `Podfile`
```ruby
pod 'ee-x/cpp'
```

## Initialization
Modify `AppDelegate.cpp`
```cpp
#include <ee/Core.hpp>

bool AppDelegate::applicationDidFinishLaunching() {
    // Call this method only once.
    ee::PluginManager::initializePlugins<ee::Library::Cocos>();
    return true;
}
```
