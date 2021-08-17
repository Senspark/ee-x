# Getting Started
## Requirements
- Cocos Creator v2: <https://www.cocos.com/en/creator/download>
- Android NDK r21+: <https://developer.android.com/ndk/downloads>
- Xcode 11+: <https://developer.apple.com/download/more>
- Android Studio 4+: <https://developer.android.com/studio>
- Android SDK: 17+
- iOS SDK: 12.0+
- macOS SDK: 10.14+

## Configuration
Install TypeScript package
```
npm install @senspark/ee-x
```

### Android
Modify `build.gradle`
```java
repositories {
    mavenCentral()
}

dependencies {
    implementation 'com.senspark.ee:core:2.10.2'
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
pod 'ee-x/cpp', '2.10.2'
```

## Initialization
```ts
import * as ee_x from '@senspark/ee-x';

// Call this method only once.
ee_x.PluginManager.initializePlugins();
```
