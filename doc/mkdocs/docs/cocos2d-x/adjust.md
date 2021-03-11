# Adjust
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:adjust:2.6.4'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/adjust', '2.6.4'
pod 'Adjust', :modular_headers => true
```

## Basic usage
Initialization
```cpp
#include <ee/Cpp.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IAdjust>();
plugin->initialize(ee::AdjustConfig()
    .setToken("your_app_token")
    .setEnvironment(ee::AdjustEnvironment::Production));
```

Set push token
```cpp
plugin->setPushToken("your_push_token");
```