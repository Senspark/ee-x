# Utils
## Platform Utils
```cpp
#include <ee/Cpp.hpp>

// Checks whether the specified application is installed.
auto applicationId = "com.senspark.ee.test";
auto isInstalled = ee::Platform::isApplicationInstalled(applicationId);

// Attempts to open the specified application.
auto applicationId = "com.senspark.ee.test";
ee::Platform::openApplication(applicationId);

// Gets the current application information.
auto applicationId = ee::Platform::getApplicationId();
auto applicationName = ee::Platform::getApplicationName();
auto versionName = ee::Platform::getVersionName();
auto versionCode = ee::Platform::getVersionCode();

// Gets the SHA1 signature (Android only).
auto signature = ee::Platform::getSHA1CertificateFingerprint();

// Gets activity (Android) or view (iOS) size in pixels.
auto [width, height] = ee::Platform::getViewSize();

// Gets the full screen size in pixels (including bars).
auto [width, height] = ee::Platform::getScreenSize();

// Gets the unique ID for the current device.
ee::noAwait([]() -> ee::Task<> {
    auto deviceId = co_await ee::Platform::getDeviceId();
});

// Composes and sends an email.
ee::Platform::sendMail("feedback@senspark.com", "ee-x test app", "Hello, ");

// Checks the current internet connection.
ee::noAwait([]() -> ee::Task<> {
    auto isAvailable = co_await ee::Platform::testConnection("www.google.com", 1.0f);
});
```

## Thread Utils
```cpp
#include <ee/Cpp.hpp>

// Runs an action on Cocos2d-x thread.
ee::Thread::runOnLibraryThread([] {
    //
});

// Runs an action on UI thread (Android) or Main thread (iOS).
ee::Thread::runOnMainThread([] {
    //
});
```

## Other Utils
```cpp
#include <ee/Cpp.hpp>

// Runs an async action inside a synced action.
ee::noAwait([]() -> ee::Task<> {
    co_await ee::Delay(1.0f);
});
```