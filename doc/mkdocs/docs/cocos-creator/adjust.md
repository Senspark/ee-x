# Adjust
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:adjust:2.7.0'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/adjust', '2.7.0'
pod 'Adjust', :modular_headers => true
```

## Basic usage
Initialization
```ts
import * as ee_x from '@senspark/ee-x';

const plugin = ee_x.PluginManager.createPlugin<ee_x.IAdjust>(ee_x.Plugin.Adjust);
const config = new ee_x.AdjustConfig();
config.token = "your_app_token";
config.environment = ee_x.AdjustEnvironment.Production;
plugin.initialize(config);
```

Set push token
```ts
plugin.setPushToken("your_push_token");
```