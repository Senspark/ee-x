# Play
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:play:2.0.5'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/play', '2.0.5'
```

## Basic usage
Initialization
```cpp
#include <ee/Cpp.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IPlay>();
```

Login / logout
```cpp
ee::noAwait([plugin]() -> ee::Task<> {
    auto result = co_await plugin->logIn();
    // Handle result.
});

ee::noAwait([plugin]() -> ee::Task<> {
    auto result = co_await plugin->logOut();
    // Handle result.
});
```