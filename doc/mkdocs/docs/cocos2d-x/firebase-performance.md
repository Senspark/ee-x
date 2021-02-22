# Firebase Performance
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:firebase-performance:2.5.0'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/firebase-performance', '2.5.0'
```

## Basic usage
Initialization
```cpp
#include <ee/Cpp.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IFirebasePerformance>();
co_await plugin->initialize();
```

Starts a trace
```cpp
auto trace = plugin->newTrace("default");
trace->start();
```