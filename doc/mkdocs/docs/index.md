# Getting Started
## Configuration
### Android
Clone the repository and let `PATH` be where you cloned the repository to

Modify `settings.gradle`
```java
include ':ee-x-core'
project(':ee-x-core').projectDir = new File('PATH/ee-x/proj.android_studio/ee-x-core')
```

Modify `build.gradle`
```java
dependencies {
    implementation project(':ee-x-core')
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
$(call import-add-path, PATH)
$(call import-module, ee-x)
LOCAL_STATIC_LIBRARIES += ee_x_core
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
add_subdirectory(PATH/ee-x ${PROJECT_BINARY_DIR}/ee-x)
target_link_libraries(${PROJECT_NAME} ee_x_core)
```

### iOS
Install CocoaPods and modify `Podfile`
```ruby
pod ee-x/core, :git => 'https://github.com/Senspark/ee-x'
```

## Initialization
Modify `AppDelegate.cpp`
```cpp
#include <ee/Core.hpp>

bool AppDelegate::applicationDidFinishLaunching() {
    ee::PluginManager::initializePlugins<ee::Library::Cocos>();
    return true;
}
```
