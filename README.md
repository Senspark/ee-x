# EE-X library

*Author: Hai Hoang*

[![Build Status](https://travis-ci.org/Senspark/ee-x.svg?branch=master)](https://travis-ci.org/Senspark/ee-x)[![Codacy Badge](https://api.codacy.com/project/badge/Grade/0d42e95ca69c43659475a4e7ed3a36a6)](https://www.codacy.com/app/Senspark/ee-x?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Senspark/ee-x&amp;utm_campaign=Badge_Grade)

## Modules

- [Crashlytics + Answers](#crashlytics) [SDK](https://fabric.io/home)

- [Local notifications](#local-notifications)

- [Firebase](#firebase) [SDK](firebase.google.com/docs/cpp/setup)

  - [Analytics](#firebase-analytics)
  
  - [Messaging](#firebase-messaging-incomplete)
  
  - [Remote Config](#firebase-remote-config)
  
  - [Storage](#firebase-storage)

- [AdMob](#admob) [SDK](https://developers.google.com/admob/)

- [AppLovin](#applovin)

- [Facebook Ads](#facebook-ads) [SDK](https://developers.facebook.com/docs/audience-network/getting-started)

- [ironSource](#ironsource) [SDK](http://developers.ironsrc.com/ironsource-mobile/android/android-sdk)

- [Unity Ads](#unity-ads) [SDK](https://github.com/Unity-Technologies)

- [Vungle](#vungle)

## Getting started

#### Android

- Clone the repository.

- Let `CLONE_PATH` be where you cloned the repository to.

- Modify `Android.mk`:

```
$(call import-add-path, CLONE_PATH)
$(call import-module, ee-x/jni)
```

- Modify `main.cpp`:

```
#include <jni.h>
#include <ee/Core.hpp>

void cocos_android_app_init(JNIEnv* env) {
    ...
    JavaVm* vm;
    env->GetJavaVm(&vm);
    ee::JniUtils::setVm(vm);
}
```

- Modify `settings.gradle`:

```
include ':ee-x-core'
project(':ee-x-core').projectDir = new File('CLONE_PATH/ee-x/proj.android_studio/ee-x-core')
```

- Add or modify `MyApplication.java`:

```
import com.ee.core.PluginManager;

public MyApplication extends Application {
    @Override
    public void onCreate() {
        PluginManager.getInstance().initializePlugins(this);
    }
}
```

- Modify `AppActivity.java`:

```
import com.ee.core.PluginManager;

public class AppActivity extends Cocos2dxActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        PluginManager.getInstance().onCreate(this);
    }

    @Override
    protected void onStart() {
        PluginManager.getInstance().onStart();
    }
    
    @Override
    protected void onStop() {
        PluginManager.getInstance().onStop();
    }

    @Override
    protected void onResume() {
        PluginManager.getInstance().onResume();
    }
    
    @Override
    protected void onPause() {
        PluginManager.getInstance().onPause();
    }
    
    @Override
    protected void onDestroy() {
        PluginManager.getInstance().onDestroy();
    }
    
    @Override
    public void onBackPressed() {
        if (PluginManager.getInstance().onBackPressed()) {
            return;
        }
    }
    
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (PluginManager.getInstance().onActivityResult(requestCode, resultCode, data)) {
            return;
        }
    }
}
```

#### iOS & macOS

- Install pod.

- Modify `AppController.mm`:

```
#include <ee/Core.hpp>

- (BOOL)application:(UIApplication*)application
    didFinishLaunchingWithOptions:(NSDictionary*)launchOptions {
    [[EEPluginManager getInstance] initializePlugins];
}
```

## Crashlytics

[Go to top](#modules)

- Supported platforms: Android, iOS, macOS.

#### Android

- Modify `Android.mk`:

```
LOCAL_STATIC_LIBRARIES += ee_x_crashlytics_static
```

- Modify `settings.gradle`:

```
include ':ee-x-crashlytics'
project(':ee-x-crashlytics').projectDir = new File('CLONE_PATH/ee-x/proj.android_studio/ee-x-crashlytics')
```

- Modify `build.gradle`:

```
dependencies {
    compile project(':ee-x-crashlytics')
}
```

- Add or modify `MyApplication.java`:

```
import com.ee.crashlytics.Crashlytics;

public class MyApplication extends Application {
    @Override
    public void onCreate() {
        PluginManager.getInstance().addPlugin(new Crashlytics(this));
    }
}
```

#### iOS & macOS

- Modify `Podfile`:

```
pod 'ee-x/crashlytics', :git => 'https://github.com/Senspark/ee-x'
```

- Modify `AppController.mm`:

```
#include <ee/Core.hpp>

- (BOOL)application:(UIApplication*)application
    didFinishLaunchingWithOptions:(NSDictionary*)launchOptions {
    [[EEPluginManager getInstance] addPlugin:@"Crashlytics"];
    ...
}
```

#### Sample C++ class:


```
// CrashlyticsAgent.hpp

#include <memory> // std::unique_ptr.
#include <string> // std::string.

#include <ee/CrashlyticsFwd.hpp>

class CrashlyticsAgent {
public:
    static CrashlyticsAgent* getInstance();
    
    void initialize();    
    void causeCrash();
    void causeException();
    void logDebug(const std::string& message);
    void logInfo(const std::string& message);
    void logError(const std::string& message);
    
private:
    std::unique_ptr<ee::Crashlytics> protocol_;
};
```

```
// CrashlyticsAgent.cpp

#include "CrashlyticsAgent.hpp"

#include <ee/Crashlytics.hpp>
#include <ee/Core.hpp>

CrashlyticsAgent* CrashlyticsAgent::getInstance() {
    static CrashlyticsAgent sharedInstance;
    return &sharedInstance;
}

void CrashlyticsAgent::initialize() {
    protocol_ = std::make_unique<ee::Crashlytics>();
}

void CrashlyticsAgent::causeCrash() {
    protocol_->causeCrash();
}

void CrashlyticsAgent::causeException() {
    protocol_->causeException();
}

void CrashlyticsAgent::logDebug(const std::string& message) {
    protocol_->log(ee::LogLevel::Debug, "your_log_tag", message);
}

void CrashlyticsAgent::logInfo(const std::string& message) {
    protocol_->log(ee::LogLevel::Info, "your_log_tag", message);
}

void CrashlyticsAgent::logError(const std::string& message) {
    protocol_->log(ee::LogLevel::Error, "your_log_tag", message);
}
```

## Local Notifications

[Go to top](#modules)

- Supported platforms: Android, iOS.

#### Android

- Notes: `mipmap/ic_launcher` (or `mipmap/icon_silhouette` for API 21 and above) will be used as icon for notifications.

- Modify `Android.mk`:

```
LOCAL_STATIC_LIBRARIES += ee_x_notification_static
```

- Modify `settings.gradle`:

```
include ':ee-x-notification'
project(':ee-x-notification').projectDir = new File('CLONE_PATH/ee-x/proj.android_studio/ee-x-notification')
```

- Modify `build.gradle`:

```
dependencies {
    compile project(':ee-x-notification')
}
```

- Add or modify `MyApplication.java`:

```
import com.ee.notification.Notification;

public class MyApplication extends Application {
    @Override
    public void onCreate() {
        PluginManager.getInstance().addPlugin(new Notification(this));
    }
}
```

#### iOS

- Modify `Podfile`:

```
pod 'ee-x/notification', :git => 'https://github.com/Senspark/ee-x'
```

- Modify `AppController.mm`:

```
#include <ee/Core.hpp>

- (BOOL)application:(UIApplication*)application
    didFinishLaunchingWithOptions:(NSDictionary*)launchOptions {
    [[EEPluginManager getInstance] addPlugin:@"Notification"];
    ...
}
```

#### Sample C++ class:


```
// NotificationAgent.hpp

#include <memory> // std::unique_ptr.

#include <ee/NotificationFwd.hpp>

class NotificationAgent {
public:
    static NotificationAgent* getInstance();
    
    void initialize();
    
    /// Should be called when entering background.
    /// E.g. AppDelegate::applicationDidEnterBackground.
    void scheduleAll();
    
    /// Should be called when entering foreground.
    /// E.g. AppDelegate::applicationWillEnterForeground.
    void unscheduleAll();
    
private:
    std::unique_ptr<ee::Notification> protocol_;
};
```

```
// NotificationAgent.cpp

#include "NotificationAgent.hpp"

#include <ee/Notification.hpp>

namespace tag {
constexpr int notification_0 = 0;
constexpr int notification_1 = 1;
} // namespace tag

NotificationAgent* NotificationAgent::getInstance() {
    static NotificationAgent sharedInstance;
    return &sharedInstance;
}

void NotificationAgent::initialize() {
    protocol_ = std::make_unique<ee::Notification>();
}

void NotificationAgent::scheduleAll() {
    // Delayed 5 minutes.
    protocol_->schedule(ee::NotificationBuilder()
                            .setTitle("Title 1")
                            .setBody("Body 2")
                            .setDelay(300)
                            .setTag(tag::notification_0));
    
    // Delayed 10 minutes and repeat every 5 minutes.
    protocol_->schedule(ee::NotificationBuilder()
                            .setTitle("Title 2")
                            .setBody("Body 2")
                            .setDelay(600)
                            .setInterval(300)
                            .setTag(tag::notification_1));
}

void NotificationAgent::unscheduleAll() {
    protocol_->unschedule(tag::notification_0);
    protocol_->unschedule(tag::notification_1);
    
    // Also clear all notifications.
    protocol_->clearAll();
}
```

## Firebase

[Go to top](#modules)

- Supported platforms: Android, iOS, macOS.

### Common modifications:

- Modify `build.gradle`:

```
dependencies {
    compile 'com.google.android.gms:play-services-base:11.0.4'
}
```

- Modify `AppActivity.java`:

```
import com.ee.firebase.core.Firebase;

public class AppActivity extends Cocos2dxActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        PluginManager.getInstance().addPlugin(new Firebase(this));
    }
}
```

### Firebase Analytics

[Go to top](#modules)

#### Android

- Modify `Android.mk`:

```
LOCAL_STATIC_LIBRARIES += ee_x_firebase_analytics_static
```

- Modify `settings.gradle`:

```
include ':ee-x-firebase-analytics'
project(':ee-x-firebase-analytics').projectDir = new File('CLONE_PATH/ee-x/proj.android_studio/ee-x-analytics')
```

- Modify `build.gradle`:

```
dependencies {
    compile project(':ee-x-firebase-analytics')
}
```

#### iOS & macOS

- Modify `Podfile`:

```
pod 'ee-x/firebase-analytics', :git => 'https://github.com/Senspark/ee-x'
```

### Firebase Messaging (incomplete)

[Go to top](#modules)

#### Android

- Modify `Android.mk`:

```
LOCAL_STATIC_LIBRARIES += ee_x_firebase_messaging_static
```

- Modify `settings.gradle`:

```
include ':ee-x-firebase-messaging'
project(':ee-x-firebase-messaging').projectDir = new File('CLONE_PATH/ee-x/proj.android_studio/ee-x-messaging')
```

- Modify `build.gradle`:

```
dependencies {
    compile project(':ee-x-firebase-messaging')
}
```

#### iOS & macOS

- Modify `Podfile`:

```
pod 'ee-x/firebase-messaging', :git => 'https://github.com/Senspark/ee-x'
```

### Firebase Remote Config

[Go to top](#modules)

#### Android

- Modify `Android.mk`:

```
LOCAL_STATIC_LIBRARIES += ee_x_firebase_remote_config_static
```

- Modify `settings.gradle`:

```
include ':ee-x-firebase-remote-config'
project(':ee-x-firebase-remote-config').projectDir = new File('CLONE_PATH/ee-x/proj.android_studio/ee-x-remote-config')
```

- Modify `build.gradle`:

```
dependencies {
    compile project(':ee-x-firebase-remote-config')
}
```

#### iOS & macOS

- Modify `Podfile`:

```
pod 'ee-x/firebase-remote-config', :git => 'https://github.com/Senspark/ee-x'
```

### Firebase Storage

[Go to top](#modules)

#### Android

- Modify `Android.mk`:

```
LOCAL_STATIC_LIBRARIES += ee_x_firebase_storage_static
```

- Modify `settings.gradle`:

```
include ':ee-x-firebase-storage'
project(':ee-x-firebase-storage').projectDir = new File('CLONE_PATH/ee-x/proj.android_studio/ee-x-storage')
```

- Modify `build.gradle`:

```
dependencies {
    compile project(':ee-x-firebase-storage')
}
```

#### iOS & macOS

- Modify `Podfile`:

```
pod 'ee-x/firebase-storage', :git => 'https://github.com/Senspark/ee-x'
```

## AppLovin

[Go to top](#modules)

- Supported platforms: Android, iOS.

#### Android

- Modify `Android.mk`:

```
LOCAL_STATIC_LIBRARIES += ee_x_applovin_static
```

- Modify `settings.gradle`:

```
include ':ee-x-applovin'
project(`:ee-x-applovin').projectDir = new File('CLONE_PATH/ee-x/proj.android_studio/ee-x-applovin')
```

- Modify `build.gradle`:

```
dependencies {
    compile project(':ee-x-applovin')
}
```

- Add or modify `MyApplication.java`:

```
import com.ee.applovin.AppLovin;

public class MyApplication extends Application {
    @Override
    public void onCreate() {
        PluginManager.getInstance().addPlugin(new AppLovin(this));
    }
}
```

#### iOS

- Modify `Podfile`:

```
pod 'ee-x/applovin', :git => 'https://github.com/Senspark/ee-x'
```

#### Sample

- See `AppLovin.cpp` and `AppLovin.hpp` in test project.

## AdMob

[Go to top](#modules)

- Supported platforms: Android, iOS.

#### Android

- Modify `Android.mk`:

```
LOCAL_STATIC_LIBRARIES += ee_x_admob_static
```

- Modify `settings.gradle`:

```
include ':ee-x-admob'
project(`:ee-x-admob').projectDir = new File('CLONE_PATH/ee-x/proj.android_studio/ee-x-admob')
```

- Modify `build.gradle`:

```
dependencies {
    compile project(':ee-x-admob')
}
```

- Add or modify `MyApplication.java`:

```
import com.ee.admob.AdMob;

public class MyApplication extends Application {
    @Override
    public void onCreate() {
        PluginManager.getInstance().addPlugin(new AdMob(this));
    }
}
```

#### iOS

- Modify `Podfile`:

```
pod 'ee-x/admob', :git => 'https://github.com/Senspark/ee-x'
```

#### Sample

- See `AdMob.cpp` and `AdMob.hpp` in test project.

## Facebook Ads

[Go to top](#modules)

- Supported platforms: Android, iOS.

#### Android

- Modify `Android.mk`:

```
LOCAL_STATIC_LIBRARIES += ee_x_facebook_ads_static
```

- Modify `settings.gradle`:

```
include ':ee-x-facebook-ads'
project(':ee-x-facebook-ads').projectDir = new File('CLONE_PATH/ee-x/proj.android_studio/ee-x-facebook-ads')
```

- Modify `build.gradle`:

```
dependencies {
    compile project(':ee-x-facebook-ads')
}
```

- Add or modify `MyApplication.java`:

```
import com.ee.facebook.FacebookAds;

public class MyApplication extends Application {
    @Override
    public void onCreate() {
        PluginManager.getInstance().addPlugin(new FacebookAds(this));
    }
}
```

#### iOS

- Modify `Podfile`:

```
pod 'ee-x/facebook-ads', :git => 'https://github.com/Senspark/ee-x'
```

#### Sample

- See `FacebookAds.cpp` and `FacebookAds.hpp` in test project.

## ironSource

[Go to top](#modules)

- Supported platforms: Android, iOS.

#### Android

- Modify `Android.mk`:

```
LOCAL_STATIC_LIBRARIES += ee_x_ironsource_static
```

- Modify `settings.gradle`:

```
include ':ee-x-ironsource'
project(':ee-x-ironsource').projectDir = new File('CLONE_PATH/ee-x/proj.android_studio/ee-x-ironsource')
```

- Modify `build.gradle`:

```
dependencies {
    compile project(':ee-x-ironsource')
}
```

- Add or modify `MyApplication.java`:

```
import com.ee.ironsource.IronSource;

public class MyApplication extends Application {
    @Override
    public void onCreate() {
        PluginManager.getInstance().addPlugin(new IronSource(this));
    }
}
```

#### iOS

- Modify `Podfile`:

```
pod 'ee-x/ironsource', :git => 'https://github.com/Senspark/ee-x'
```

#### Sample

- See `IronSource.cpp` and `IronSource.hpp` in test project.

## Unity Ads

[Go to top](#modules)

- Supported platforms: Android, iOS.

#### Android

- Modify `Android.mk`:

```
LOCAL_STATIC_LIBRARIES += ee_x_unity_ads_static
```

- Modify `settings.gradle`:

```
include ':ee-x-unity-ads'
project(':ee-x-unity-ads').projectDir = new File('CLONE_PATH/ee-x/proj.android_studio/ee-x-unity-ads')
```

- Modify `build.gradle`:

```
dependencies {
    compile project(':ee-x-unity-ads')
}
```

- Add or modify `MyApplication.java`:

```
import com.ee.unityads.UnityAds;

public class MyApplication extends Application {
    @Override
    public void onCreate() {
        PluginManager.getInstance().addPlugin(new UnityAds());
    }
}
```

#### iOS

- Modify `Podfile`:

```
pod 'ee-x/unity-ads', :git => 'https://github.com/Senspark/ee-x'
```

#### Sample

- See `UnityAds.cpp` and `UnityAds.hpp` in test project.

## Vungle

[Go to top](#modules)

- Supported platforms: Android, iOS.

#### Android

- Modify `Android.mk`:

```
LOCAL_STATIC_LIBRARIES += ee_x_vungle_ads_static
```

- Modify `settings.gradle`:

```
include ':ee-x-vungle'
project(':ee-x-vungle').projectDir = new File('CLONE_PATH/ee-x/proj.android_studio/ee-x-vungle')
```

- Modify `build.gradle`:

```
dependencies {
    compile project(':ee-x-vungle')
}
```

- Add or modify `MyApplication.java`:

```
import com.ee.vungle.Vungle;

public class MyApplication extends Application {
    @Override
    public void onCreate() {
        PluginManager.getInstance().addPlugin(new Vungle(this));
    }
}
```

#### iOS

- Modify `Podfile`:

```
pod 'ee-x/vungle', :git => 'https://github.com/Senspark/ee-x'
```

#### Sample

- See `Vungle.cpp` and `Vungle.hpp` in test project.