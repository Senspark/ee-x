# Firebase Crashlytics
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:firebase-crashlytics:2.0.2'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/firebase-crashlytics'
```

## Basic usage
Initialization
```cpp
#include <ee/Cpp.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IFirebaseCrashlytics>();
```

Logs a message
```cpp
plugin->log("message");
```