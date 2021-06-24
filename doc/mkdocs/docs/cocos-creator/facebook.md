# Facebook
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:facebook:2.9.0'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/facebook', '2.9.0'
```

## Basic usage
Initialization
```ts
import * as ee_x from '@senspark/ee-x';

const plugin = ee_x.PluginManager.create<ee_x.IFacebook>(ee_x.Plugin.Facebook);
```

Login and logout
```ts
// Checks whether the user is logged in.
const isLoggedIn = plugin.isLoggedIn;

// Logs in.
const permissions = ['public_profile'];
const result = await plugin.logIn(permissions);
if (result.successful) {
    // Handle success.
} else if (result.canceled) {
    // Handle cancelation.
} else {
    // Handle failure.
    const message = result.errorMessage;
}

// Gets user ID.
const userId = plugin.accessToken?.userId;

// Logs out.
await plugin.logOut();
```

Graph request
```ts
const request = new ee_x.FacebookGraphRequest()
    .setPath("graph_path")
    .setParameter("param1", "value1")
    .setParameter("param2", "value2");
const result = await plugin.graphRequest(request);
if (result.successful) {
    // Handle success.
    const response = result.response;
} else {
    // Handle failure.
    const message = result.errorMessage;
}
```