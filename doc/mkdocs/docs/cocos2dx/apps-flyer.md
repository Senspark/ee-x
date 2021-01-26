# AppsFlyer
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:apps-flyer:2.3.1'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/apps-flyer', '2.3.1'
```

## Basic usage
Initializes with dev key and app ID
```cpp
#include <ee/Cpp.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IAppsFlyer>();
plugin->initialize("your_dev_key", "your_app_id");
```

Start tracking
```cpp
plugin->startTracking();
```