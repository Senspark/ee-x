# Facebook
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:facebook:2.0.4'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/facebook', '2.0.4'
```

## Basic usage
Initialization
```cpp
#include <ee/Cpp.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IFacebook>();
```