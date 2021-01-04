# Soomla Store (Legacy)
## Configuration
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:soomla-store:2.1.1'
}
```

#### For `ndk-build` users
Modify `Android.mk`
```
LOCAL_STATIC_LIBRARIES += soomla_store
```

#### For `cmake` users
Modify `CMakeLists.txt`
```
target_link_libraries(${PROJECT_NAME} soomla_store)
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/soomla-store', '2.1.1'
```