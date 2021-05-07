# Firebase Analytics
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:firebase-analytics:2.7.3'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/firebase-analytics', '2.7.3'
```

## Basic usage
Initialization
```cpp
#include <ee/Cpp.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IFirebaseAnalytics>();
co_await plugin->initialize();
```

Sets user property
```cpp
plugin->setUserProperty("key", "value")
```

Tracks current screen
```cpp
plugin->trackScreen("current_screen");
```

Logs an event
```cpp
plugin->logEvent("event_name", {
    {"key1", 1},
    {"key2", 2.0},
    {"key3", "value3"}
});
```