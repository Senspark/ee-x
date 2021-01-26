# Firebase Performance
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:firebase-performance:2.3.1'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/firebase-performance', '2.3.1'
```

## Basic usage
Initialization
```cpp
#include <ee/Cpp.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IFirebasePerformance>();
auto trace = plugin->startTrace("default");
trace->start();
```