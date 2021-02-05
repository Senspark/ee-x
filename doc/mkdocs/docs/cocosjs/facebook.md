# Facebook
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:facebook:2.4.0'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/facebook', '2.4.0'
```

## Basic usage
Initialization
```ts
import * as ee_x from '@senspark/ee-x';

const plugin = ee_x.PluginManager.create<ee_x.IFacebook>(ee_x.Plugin.Facebook);
```