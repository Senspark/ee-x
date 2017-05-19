Firebase C++ SDK
================

The Firebase C++ SDK provides C++ interfaces for the following Firebase
services on *iOS* and *Android*:

* AdMob
* Firebase Analytics
* Firebase App Invites
* Firebase Authentication
* Firebase Cloud Messaging
* Firebase Dynamic Links
* Firebase Realtime Database
* Firebase Remote Config
* Firebase Storage

## Stub Implementations

Stub (non-functional) implementations are provided for convenience when
building for Windows, OSX and Linux so the developer does not need to
conditionally compile code when also targeting the desktop.

Directory Structure
-------------------

The following table provides an overview of the Firebase C++ SDK directory
structure.

| Directories                   | Contents                                    |
|-------------------------------|---------------------------------------------|
| include                       | C++ headers                                 |
| frameworks/ios/ARCH           | iOS frameworks (compiled against libc++)    |
|                               | A multi-architecture framework is \         |
|                               | provided in the *universal* directory.      |
| libs/ios/ARCH                 | OSX static libraries (compiled against \    |
|                               | libc++)                                     |
|                               | Multi-architecture libraries are  \         |
|                               | provided in the *universal* directory.      |
| libs/android/ARCH/STL         | Android (GCC 4.8+ compatible) static \      |
|                               | libraries for each architecture and STL \   |
|                               | variant.                                    |
|                               | _STL variants available:_                   |
|                               | * `c++`: LLVM libc++ runtime (recommended)  |
|                               | * `gnustl`: GNU STL                         |
|                               | * `stlport`: STLport runtime                |
|                               | More information can be found in the \      |
|                               | [NDK C++ Helper Runtimes](https://developer.android.com/ndk/guides/cpp-support.html#runtimes) \ |
|                               | documentation.                              |
| *Stub Implementations*        |                                             |
| libs/darwin                   | OSX static libraries (stub \                |
|                               | implementation, compiled against libc++)    |
| frameworks/darwin             | OSX frameworks (stub implementations, \     |
|                               | compiled against libc++)                    |
| libs/linux                    | Linux static libraries (GCC 4.7+, libc++).  |
| libs/windows                  | Windows static libraries \                  |
|                               | (stub implementation, MSVC 2015+)           |

Library / Framework Dependencies
--------------------------------

Each feature has dependencies upon libraries in this SDK and components
distributed as part of the core Firebase
[iOS SDK](https://firebase.google.com/docs/ios/setup) and
[Android SDK](https://firebase.google.com/docs/android/setup).

### Android Dependencies

| Feature                  | Required Libraries and Gradle Packages           |
|--------------------------|--------------------------------------------------|
| Firebase AdMob           | libadmob.a<bt> \                                 |
|                          | libapp.a<br> \                                   |
|                          | com.google.android.gms:play-services-ads \       |
|                          | (Maven package)                                  |
| Firebase Analytics       | libanalytics.a<br> \                             |
|                          | libapp.a<br> \                                   |
|                          | com.google.android.gms:firebase-analytics \      |
|                          | (Maven package)                                  |
| Firebase Authentication  | libauth.a<br>  \                                 |
|                          | libapp.a<br> \                                   |
|                          | com.google.android.gms:firebase-auth \           |
|                          | (Maven package)                                  |
| Firebase Invites and \   | libinvites.a<br> \                               |
| Dynamic Links            | libapp.a<br> \                                   |
|                          | com.google.android.gms:firebase-invites \        |
|                          | (Maven package)                                  |
| Firebase Messaging       | libmessaging.a<br> \                             |
|                          | libapp.a<br> \                                   |
|                          | com.google.android.gms:firebase-messaging \      |
|                          | (Maven package)<br> \                            |
|                          | libmessaging_java.jar (Android service)          |
| Firebase Realtime \      | libdatabase.a<br> \                              |
| Database                 | libauth.a<br> \                                  |
|                          | libapp.a<br> \                                   |
|                          | com.google.android.gms:firebase-database  \      |
|                          | (Maven package)<br> \                            |
|                          | com.google.android.gms:firebase-auth \           |
|                          | (Maven package)                                  |
| Firebase Remote Config   | libremote_config.a<br> \                         |
|                          | libapp.a<br> \                                   |
|                          | com.google.android.gms:firebase-config \         |
|                          | (Maven package)                                  |
| Firebase Storage         | libstorage.a<br> \                               |
|                          | libauth.a<br> \                                  |
|                          | libapp.a<br> \                                   |
|                          | com.google.android.gms:firebase-storage   \      |
|                          | (Maven package)<br> \                            |
|                          | com.google.android.gms:firebase-auth \           |
|                          | (Maven package)                                  |

### iOS Dependencies

iOS users can include either frameworks or static libraries depending upon
their preferred build environment.

#### Frameworks

| Feature                      | Required Frameworks and Cocoapods            |
|------------------------------|----------------------------------------------|
| Firebase AdMob               | firebase_admob.framework<br> \               |
|                              | firebase.framework<br> \                     |
|                              | Firebase/Core Cocoapod<br> \                 |
|                              | Google-Mobile-Ads-SDK Cocoapod               |
| Firebase Analytics           | firebase_analytics.framework<br> \           |
|                              | firebase.framework<br> \                     |
|                              | Firebase/Core Cocoapod                       |
| Firebase Authentication      | firebase_auth.framework<br>  \               |
|                              | firebase.framework<br> \                     |
|                              | Firebase/Auth Cocoapod                       |
| Firebase Invites and \       | firebase_invites.framework<br> \             |
| Dynamic Links                | firebase.framework<br> \                     |
|                              | Firebase/Invites Cocoapod                    |
| Firebase Cloud Messaging     | firebase_messaging.framework<br> \           |
|                              | firebase.framework<br> \                     |
|                              | Firebase/Messaging Cocoapod                  |
| Firebase Realtime Database   | firebase_database.framework<br> \            |
|                              | firebase_auth.framework<br> \                |
|                              | firebase.framework<br> \                     |
|                              | Firebase/Database Cocoapod<br> \             |
|                              | Firebase/Auth Cocoapod<br>                   |
| Firebase Remote Config       | firebase_remote_config.framework<br> \       |
|                              | firebase.framework<br> \                     |
|                              | Firebase/RemoteConfig Cocoapod               |
| Firebase Storage             | firebase_storage.framework<br> \             |
|                              | firebase_auth.framework<br> \                |
|                              | firebase.framework<br> \                     |
|                              | Firebase/Storage Cocoapod<br> \              |
|                              | Firebase/Auth Cocoapod<br>                   |


#### Libraries

If you prefer to link against static libraries instead of frameworks (see the
previous section) the following table describes the libraries and Cocoapods
required for each SDK feature.

| Feature                      | Required Libraries and Cocoapods             |
|------------------------------|----------------------------------------------|
| Firebase AdMob               | libadmob.a<br> \                             |
|                              | libapp.a<br> \                               |
|                              | Firebase/Core Cocoapod<br> \                 |
|                              | Google-Mobile-Ads-SDK Cocoapod               |
| Firebase Analytics           | libanalytics.a<br> \                         |
|                              | libapp.a<br> \                               |
|                              | Firebase/Core Cocoapod                       |
| Firebase Authentication      | libauth.a<br>  \                             |
|                              | libapp.a<br> \                               |
|                              | FirebaseAuth Cocoapod                        |
| Firebase Invites and \       | libinvites.a<br> \                           |
| Dynamic Links                | libapp.a<br> \                               |
|                              | Firebase/AppInvites Cocoapod                 |
| Firebase Cloud Messaging     | libmessaging.a<br> \                         |
|                              | libapp.a<br> \                               |
|                              | Firebase/CloudMessaging Cocoapod             |
| Firebase Realtime Database   | libdatabase.a<br> \                          |
|                              | libapp.a<br> \                               |
|                              | libauth.a<br> \                              |
|                              | Firebase/Database Cocoapod<br> \             |
|                              | Firebase/Auth Cocoapod                       |
| Firebase Remote Config       | libremote_config.a<br> \                     |
|                              | libapp.a<br> \                               |
|                              | Firebase/RemoteConfig Cocoapod               |
| Firebase Storage             | libstorage.a<br> \                           |
|                              | libapp.a<br> \                               |
|                              | libauth.a<br> \                              |
|                              | Firebase/Storage Cocoapod<br> \              |
|                              | Firebase/Auth Cocoapod                       |


### Stub Implementation Dependencies

| Feature                      | Required Libraries and Gradle Packages       |
|------------------------------|----------------------------------------------|
| Firebase AdMob               | libadmob.a<br> \                             |
|                              | libapp.a<br>                                 |
| Firebase Analytics           | libanalytics.a<br> \                         |
|                              | libapp.a<br>                                 |
| Firebase Authentication      | libauth.a<br>  \                             |
|                              | libapp.a<br>                                 |
| Firebase Invites and \       | libinvites.a<br> \                           |
| Dynamic Links                | libapp.a<br>                                 |
| Firebase Cloud Messaging     | libmessaging.a<br> \                         |
|                              | libapp.a<br>                                 |
| Firebase Realtime Database   | libdatabase.a<br> \                          |
|                              | libauth.a<br> \                              |
|                              | libapp.a<br>                                 |
| Firebase Remote Config       | libremote_config.a<br> \                     |
|                              | libapp.a<br>                                 |
| Firebase Storage             | libstorage.a<br> \                           |
|                              | libauth.a<br> \                              |
|                              | libapp.a<br>                                 |

Getting Started
---------------

See our [setup guide](https://firebase.google.com/docs/cpp/setup) to get
started.

Platform Notes
--------------

### iOS Method Swizzling

On iOS, some application events (such as opening URLs and receiving
notifications) require your application delegate to implement specific methods.
For example, receiving a notification may require your application delegate to
implement `application:didReceiveRemoteNotification:`. Because each iOS
application has its own app delegate, Firebase uses _method swizzling_, which
allows the replacement of one method with another, to attach its own handlers in
addition to any you may have implemented.

The Firebase Invites and Firebase Cloud Messaging libraries need to attach
handlers to the application delegate using method swizzling. If you are using
these libraries, at load time, Firebase will identify your `AppDelegate` class
and swizzle the required methods onto it, chaining a call back to your existing
method implementation.

### Custom Android Build Systems

We currently provide generate\_xml\_from\_google\_services\_json.py to convert
google-services.json to .xml resources to be included in an Android application.
This script applies the same transformation that the Google Play Services Gradle
plug-in performs when building Android applications.  Users who don't use Gradle
(e.g ndk-build, makefiles, Visual Studio etc.) can use this script to automate
the generation of string resources.

### ProGuard on Android

Many Android build systems use
[ProGuard](https://developer.android.com/studio/build/shrink-code.html) for
builds in Release mode to shrink application sizes and protect Java source code.
If you use ProGuard, you will need to add the files in libs/android/*.pro
corresponding to the Firebase C++ libraries you are using to your ProGuard
configuration.

For example, with Gradle, build.gradle would contain:
~~~
  android {
    [...other stuff...]
    buildTypes {
      release {
        minifyEnabled true
        proguardFile getDefaultProguardFile('your-project-proguard-config.txt')
        proguardFile file(project.ext.firebase_cpp_sdk_dir + "/libs/android/app.pro")
        proguardFile file(project.ext.firebase_cpp_sdk_dir + "/libs/android/analytics.pro")
        [...and so on, for each Firebase C++ library you are using.]
      }
    }
  }
~~~

### Requiring Google Play services on Android

Many Firebase C++ libraries require
[Google Play services](https://developers.google.com/android/guides/overview) on
the user's Android device. If a Firebase C++ library returns
[`kInitResultFailedMissingDependency`](http://firebase.google.com/docs/reference/cpp/namespace/firebase)
on initialization, it means Google Play services is not available on the device
(it needs to be updated, reactivated, permissions fixed, etc.) and that Firebase
library cannot be used until the situation is corrected.

You can find out why Google Play services is unavailable (and try to fix it) by
using the functions in [`google_play_services/availability.h`](http://firebase.google.com/docs/reference/cpp/namespace/google-play-services).

Optionally, you can use
[`ModuleInitializer`](http://firebase.google.com/docs/reference/cpp/class/firebase/module-initializer)
to initialize one or more Firebase libraries, which will handle prompting the
user to update Google Play services if required.

Note: Some libraries do not require Google Play services and don't return any
initialization status. These can be used without Google Play services. The table
below summarizes whether Google Play services is required by each Firebase C++
library.

| Firebase C++ Library    | Google Play services required?    |
| ----------------------- |:---------------------------------:|
| Analytics               | Not required                      |
| AdMob                   | Not required (usually; see below) |
| Cloud Messaging         | Required                          |
| Auth                    | Required                          |
| Invites / Dynamic Links | Required                          |
| Realtime Database       | Required                          |
| Remote Config           | Required                          |
| Storage                 | Required                          |

#### A note on AdMob and Google Play services

Most versions of the Google Mobile Ads SDK for Android can work properly without
Google Play services. However, if you are using the
`com.google.android.gms:play-services-ads-lite` dependency instead of the
standard `com.google.firebase:firebase-ads` dependency, Google Play services
WILL be required in your specific case.

AdMob initialization will only return `kInitResultFailedMissingDependency` when
Google Play services is unavailable AND you are using
`com.google.android.gms:play-services-ads-lite`.


Release Notes
-------------
### 3.1.2
  - Overview
    - Bug fixes for Auth.
  - Changes
    - Auth: Fixed a crash caused by a stale memory reference when a
      firebase::auth::Auth object is destroyed and then recreated for the same
      App object.
    - Auth: Fixed potential memory corruption when AuthStateListener is
      destroyed.
### 3.1.1
  - Overview
    - Bug fixes for Auth, Invites, Messaging, and Storage, plus a general fix.
  - Changes
    - General (Android): Fixed Google Play Services updater crash when clicking
      outside of the dialog on Android 4.x devices.
    - Auth: Fixed user being invalidated when linking a credential fails.
    - Auth: Deprecated User::refresh_token().
    - Messaging: Fixed incorrectly notifying the app of a message when a
      notification is received while the app is in the background and the app
      is then opened by via the app icon rather than the notification.
    - Invites (iOS): Fixed an issue which resulted in the app delegate method
      application:openURL:sourceApplication:annotation: not being called
      when linking the invites library.
    - Storage: Fixed a bug that prevented the construction of Metadata without
      a storage reference.

### 3.1.0
  - Overview
    - Added support for multiple storage buckets in Cloud Storage for Firebase,
      and fixed a bug in Invites.
  - Changes
    - Storage: Renamed "Firebase Storage" to "Cloud Storage for Firebase".
    - Storage: Added an overload for `Storage::GetInstance()` that accepts a
      `gs://...` URL, so you can use Cloud Storage with multiple buckets.
    - Invites: (Android) Fixed an issue where invites with empty links would
      fail to be received.

### 3.0.0
  - Overview
    - Renamed some methods, fixed some bugs, and added some features.
  - Changes
    - General: Renamed and deprecated methods that were inconsistent with the
      Google C++ Style Guide. Deprecated methods will be removed in a future
      release (approximately 2-3 releases from now).
    - Analytics: Added `SetCurrentScreen()`.
    - Auth: Fixed a race condition accessing user data in callbacks.
    - Auth: (Android) Added `is_valid()` to check if a credential returned by
      `GetCredential()` is valid.
    - Invites: (Android) Added a `Fetch()` function to fetch incoming
      invitations at times other than application start. You must call this on
      Android when your app returns to the foreground (on iOS, this is handled
      automatically).
    - Messaging: Added a field to `firebase::messaging::Message` specifying
      whether the message was received when the app was in the background.
    - Messaging: (Android) Added an AAR file containing the Android manifest
      changes needed for receiving notifications. You can add this to your
      project instead of modifying the manifest directly.
    - Messaging: (iOS) Fixed regression since 2.1.1 that broke messaging
      on iOS 8 & 9 when an AppDelegate did not implement remote notification
      methods.
    - Invites: (iOS) Fixed regression since 2.1.1 that broke invites if the
      AppDelegate did not implement the open URL method.
    - Remote Config: Added support for initializing Remote Config defaults
      from `firebase::Variant` values, including binary data.

### 2.1.3
  - Overview
    - Bug fixes for Auth and Messaging, and a fix for Future callbacks.
  - Changes
    - General: Fixed a potential deadlock when running callbacks registered via
      `firebase::Future::OnCompletion()`.
    - Auth: (Android) Fixed an error in `firebase::auth::User::PhotoUri()`.
    - Messaging: (Android) Fixed an issue where a blank message would appear.
    - Messaging: (iOS) Removed hard dependency on Xcode 8.

### 2.1.2
  - Overview
    - Bug fix for AdMob on Android.
  - Changes
    - AdMob: (Android) Fixed an issue in `firebase::admob::InterstitialAd` that
      caused a crash after displaying multiple interstitial ads.

### 2.1.1
  - Overview
    - Bug fixes for Firebase Authentication, Messaging and Invites.
  - Changes
    - Auth: (Android) Fixed an issue that caused a future to never complete
      when signing in while a user is already signed in.
    - Messaging / Invites: (iOS) Fixed an issue with method swizzling that
      caused some of the application's UIApplicationDelegate methods to not
      be called.
    - Messaging: (iOS) Fixed a bug which caused a crash when initializing the
      library when building with Xcode 8 for iOS 10.

### 2.1.0
  - Overview
    - Support for Firebase Storage and minor bugfixes in other libraries.
  - Changes
    - Storage: Added the Firebase Storage C++ client library.
    - Auth: Added a check for saved user credentials when Auth is initialized.
  - Known Issues
    - Storage: On Android, pausing and resuming storage operations will cause
      the transfer to fail with the error code kErrorUnknown.

### 2.0.0
  - Overview
    - Support for AdMob Native Express Ads, Realtime Database and simplified
      the Invites API.
  - Changes
    - AdMob: Added support for AdMob Native Express Ads.
    - Auth: Added AuthStateListener class which provides notifications when a
      user is logged in or logged out.
    - Realtime Database: Added a client library.
    - Invites: Breaking change which significantly simplifies the API.
  - Known Issues
    - AdMob: When calling Initialize, the optional admob_app_id argument is
      ignored.

### 1.2.1
  - Overview
    - Bug fixes in Messaging.
  - Changes
    - Messaging: Fixed a bug that prevented Android apps from terminating
      properly.
    - Messaging: Added missing copy constructor implementation in iOS and stub
      libraries.

### 1.2.0
  - Overview
    - New features in AdMob, Authentication, Messaging, and Remote Config, a
      helper class for initialization, and bug fixes.
  - Changes
    - General: Added firebase::ModuleInitializer, a helper class to initialize
      Firebase modules and handle any missing dependency on Google Play services
      on Android.
    - AdMob: Added Rewarded Video feature. For more information, see the
      [Rewarded Video C++ guide](https://firebase.google.com/docs/admob/cpp/rewarded-video).
    - AdMob: You can now pass your AdMob App ID to firebase::admob::Initialize()
      to help reduce latency for the initial ad request.
    - AdMob: On both iOS and Android, you must call BannerView::Show() to
      display the ad. Previously, this was only required on Android.
    - AdMob: Fixed an issue where BannerView::Listener received an incorrect
      bounding box.
    - AdMob: BannerView now has a black background, rather than transparent.
    - Authentication: Implemented User::SendEmailVerification() and
      User::EmailVerified() methods on Android.
    - Invites: Fixed a bug that occurred when initializing InvitesSender and
      InvitesReceiver at the same time.
    - Invites: Fixed a potential crash at app shutdown on iOS when
      InvitesReceiver::Fetch() is pending.
    - Messaging: Added firebase::messaging::Notification and associated methods
      for retrieving the contents of a notification on Android and iOS.
    - Messaging: Added support for iOS 10 notifications.
    - Messaging: Fixed a crash that occurred on Android if Messaging was
      initialized before the native library was loaded.
    - RemoteConfig: Added GetKeys() and GetKeysByPrefix() methods, which get a
      list of the app's Remote Config parameter keys.

### 1.1.0
  - Overview
    - Minor bug fixes and new way of checking Google Play services availability.
  - Changes
    - Reverted the firebase::App changes from version 1.0.1 relating to Google
      Play services; this has been replaced with a new API.
    - Each Firebase C++ library that requires Google Play services now checks
      for its availability at initialization time. See "Requiring Google Play
      services on Android".
      - firebase::auth::GetAuth() now has an optional output parameter that
        indicates whether initialization was successful, and will return
        nullptr if not.
      - firebase::messaging::Initialize() now returns a result that indicates
        whether initialization was successful.
      - Added firebase::invites::Initialize(), which you must call once prior to
        creating InvitesSender or InvitesReceiver instances. This function
        returns a result that indicates whether initialization was successful.
      - firebase::remote_config::Initialize() now returns a result that
        indicates whether initialization was successful.
      - firebase::admob::Initialize() now returns a result that indicates
        whether initialization was successful.
    - Added utility functions to check whether Google Play services is
      available. See google_play_services::CheckAvailability() and
      google_play_services::MakeAvailable() for more information.
  - Known Issues
    - Invites: If you call InvitesReceiver::Fetch() or
      InvitesReceiver::ConvertInvitation() without first calling
      firebase::invites::Initialize(), the operation will never complete. To
      work around the issue, ensure that firebase::invites::Initialize() is
      called once before creating any InvitesReceiver instances.

### 1.0.1
  - Overview
    - Minor bug fixes.
  - Changes
    - Modified firebase::App to check for the required version of Google Play
      services on creation to prevent firebase::App creation failing if a
      user's device is out of date.  If Google Play services is out of date,
      a dialog will prompt the user to install a new version.  See
      "Requiring Google Play services on Android".  With the previous version
      (version 1.0.0) the developer needed to manually check for an up to date
      Google Play services using GoogleApiClient.
    - Fixed potential deadlock when using SetListener from a notification
      callback in firebase::admob::InterstitialAd and
      firebase::admob::BannerView on iOS.
    - Fixed race condition on destruction of admob::BannerView on Android.
    - Fixed Future handle leak.  An internal memory leak would manifest for
      objects or modules that use futures for the lifetime of the object or
      module.  For example, during the lifetime of BannerView each call to a
      method which returns a Future could potentially allocate memory which
      wouldn't be reclaimed until the BannerView is destroyed.

### 1.0.0
  - Overview
     - First public release.
       See our [setup guide](https://firebase.google.com/docs/cpp/setup) to get
       started.
  - Known Issues
     - Android armeabi libraries must be linked with gcc 4.8.
