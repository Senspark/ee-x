# Utils
## Platform Utils
```cpp
// Checks whether the specified application is installed.
auto applicationId = "com.senspark.ee.test";
auto isInstalled = ee::isApplicationInstalled(applicationId);

// Attempts to open the specified application.
auto applicationId = "com.senspark.ee.test";
ee::openApplication(applicationId);

// Gets the current application information.
auto applicationId = ee::getApplicationId();
auto applicationName = ee::getApplicationName();
auto versionName = ee::getVersionName();
auto versionCode = ee::getVersionCode();

// Gets the SHA1 signature (Android only).
auto signature = ee::getSHA1CertificateFingerprint();

// Gets activity (Android) or view (iOS) size in pixels.
auto [width, height] = ee::getViewSize();

// Gets the full screen size in pixels (including bars).
auto [width, height] = ee::getScreenSize();

// Get the unique ID for the current device.
ee::noAwait([]() -> ee::Task<> {
    auto deviceId = co_await ee::getDeviceId();
});

// Composes and sends an email.
ee::sendMail("feedback@senspark.com", "ee-x test app", "Hello, ");

// Checks the current internet connection.
ee::noAwait([]() -> ee::Task<> {
    auto isAvailable = co_await ee::testConnection("www.google.com", 1.0f);
});
```

## Thread Utils
```cpp
// Runs an action on Cocos2d-x thread.
ee::Thread::runOnLibraryThread([] {
    //
});

// Runs an action on UI thread (Android) or Main thread (iOS).
ee::runOnMainThread([] {
    //
});
```

## Other Utils
```cpp
// Runs an async action inside a synced action.
ee::noAwait([]() -> ee::Task<> {
    co_await ee::Delay(1000);
});
```