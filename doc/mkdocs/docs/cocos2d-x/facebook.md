# Facebook
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:facebook:2.7.4'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/facebook', '2.7.4'
```

## Basic usage
Initialization
```cpp
#include <ee/Cpp.hpp>

auto plugin = ee::PluginManager::createPlugin<ee::IFacebook>();
```

Login and logout
```cpp
// Checks whether the user is logged in.
auto isLoggedIn = plugin->isLoggedIn();

// Logs in.
auto permissions = {"public_profile"};
auto result = co_await plugin->logIn(permissions);
if (result.successful) {
    // Handle success.
} else if (result.canceled) {
    // Handle cancelation.
} else {
    // Handle failure.
    auto message = result.errorMessage;
}

// Gets user ID.
auto userId = plugin->getAccessToken()->getUserId();

// Logs out.
co_await plugin->logOut();
```

Graph request
```cpp
auto request = ee::FacebookGraphRequest()
    .setPath("graph_path")
    .setParameter("param1", "value1")
    .setParameter("param2", "value2");
auto result = co_await plugin->graphRequest(request);
if (result.successful) {
    // Handle success.
    auto response = result.response;
} else {
    // Handle failure.
    auto message = result.errorMessage;
}
```