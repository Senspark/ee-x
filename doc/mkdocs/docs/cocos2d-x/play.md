# Play
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:play:2.9.2'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/play', '2.9.2'
```

## Basic usage
Initialization
```cpp
#include <ee/Cpp.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IPlay>();
```

Login and logout
```cpp
auto result = co_await plugin->logIn();
// Handle result.

auto result = co_await plugin->logOut();
// Handle result.
```