# Firebase Crashlytics
## Configuration
### Android
Modify `settings.gradle`
```java
include ':ee-x-firebase-core'
include ':ee-x-firebase-crashlytics'
project(':ee-x-firebase-core').projectDir = new File('PATH/ee-x/proj.android_studio/ee-x-firebase-core')
project(':ee-x-firebase-crashlytics').projectDir = new File('PATH/ee-x/proj.android_studio/ee-x-firebase-crashlytics')
```

Modify `build.gradle`
```java
dependencies {
    implementation project(':ee-x-firebase-crashlytics')
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
LOCAL_STATIC_LIBRARIES += ee_x_firebase_crashlytics
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
target_link_libraries(${PROJECT_NAME} ee_x_firebase_crashlytics)
```

### iOS
Modify `Podfile`
```ruby
pod ee-x/firebase-crashlytics, :git => 'https://github.com/Senspark/ee-x'
```

## Basic usage
Initialization
```cpp
#include <ee/Firebase.hpp>

auto plugin = std::make_shared<ee::FirebaseCrashlytics>();
```

Logs a message
```cpp
plugin->log("message");
```