# Store
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:store:2.0.0'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/store'
```

## Basic usage
Initialization
```cpp
#include <ee/Store.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IStore>();
```