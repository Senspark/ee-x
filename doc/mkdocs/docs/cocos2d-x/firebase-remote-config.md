# Firebase Remote Config
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:firebase-remote-config:2.6.2'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/firebase-remote-config', '2.6.2'
```

## Basic usage
Initialization
```cpp
#include <ee/Cpp.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IFirebaseRemoteConfig>();
co_await plugin->initialize();
```

Fetch and activate configs
```cpp
co_await plugin->fetch(0);
co_await plugin->activate();
```

Set default configs
```cpp
co_await plugin->setDefaults({
    {"key1", true}
    {"key2", 1},
    {"key3", 2.0},
    {"key4", "value4"}
});
```

Get config values
```cpp
// Get bool value.
auto value1 = plugin->getBool("key1");

// Get long value.
auto value2 = plugin->getLong("key2");

// Get double value.
auto value3 = plugin->getDouble("key3");

// Get string value.
auto value4 = plugin->getString("key4");
```