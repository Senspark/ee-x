# Store
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:store:2.4.1'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/store', '2.4.1'
```

## Basic usage
Initialization
```cpp
#include <ee/Cpp.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IStore>();
```