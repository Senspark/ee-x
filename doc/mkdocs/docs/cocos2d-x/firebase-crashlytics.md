# Firebase Crashlytics
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:firebase-crashlytics:2.7.0'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/firebase-crashlytics', '2.7.0'
```

## Basic usage
Initialization
```cpp
#include <ee/Cpp.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IFirebaseCrashlytics>();
co_await plugin->initialize();
```

Logs a message
```cpp
plugin->log("message");
```