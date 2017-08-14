# EE-X library

*Author: Hai Hoang*

|Travis|Jenkins-Android|Jenkins-iOS|Jenkins-MacOS|
|:--:|:--:|:--:|:--:|
|[![Build Status](https://travis-ci.org/Senspark/ee-x.svg?branch=master)](https://travis-ci.org/Senspark/ee-x)|[![Build Status](http://14.161.14.97:8080/buildStatus/icon?job=ee_x/platform=android)](http://14.161.14.97:8080/job/ee_x/platform=android/)|[![Build Status](http://14.161.14.97:8080/buildStatus/icon?job=ee_x/platform=ios)](http://14.161.14.97:8080/job/ee_x/platform=ios)|[![Build Status](http://14.161.14.97:8080/buildStatus/icon?job=ee_x/platform=macos)](http://14.161.14.97:8080/job/ee_x/platform=macos)|

## Modules

- Crashlytics

- Local notifications

- Firebase

## Getting started

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

- Modify `AppActivity.java`:

```
import com.ee.core.PluginManager;

public class AppActivity extends Cocos2dxActivity {
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

## Crashlytics

- Supported platforms: Android, iOS, Mac OSX.

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

- Modify `AppActivity.java`:

```
import com.ee.crashlytics.Crashlytics;

public class AppActivity extends Cocos2dxActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        PluginManager.getInstance().addPlugin(new Crashlytics(this));
    }
}
```

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

- Sample C++ class:

 
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

- Supported platforms: Android, iOS.

- Notes for Android: `mipmap/ic_launcher` (or `mipmap/icon_silhouette` for API 21 and above) will be used as icon for notifications.

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

- Modify `AppActivity.java`:

```
import com.ee.notification.Notification;

public class AppActivity extends Cocos2dxActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        PluginManager.getInstance().addPlugin(new Notification(this));
    }
}
```

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

- Sample C++ class:

 
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

- Supported platforms: Android, iOS, Mac OSX.

### Common modifications:

- Modify `build.gradle`:

```
dependencies {
    compile 'com.google.android.gms:play-services-base:11.0.4'
}
```

- Modify `AppActivity.java`:

```
import com.ee.firebase.Firebase;

public class AppActivity extends Cocos2dxActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        PluginManager.getInstance().addPlugin(new Firebase(this));
    }
}
```

### Analytics

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

- Modify `Podfile`:

```
pod 'ee-x/firebase-analytics', :git => 'https://github.com/Senspark/ee-x'
```

### Messaging (incomplete)

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

- Modify `Podfile`:

```
pod 'ee-x/firebase-messaging', :git => 'https://github.com/Senspark/ee-x'
```

### Remote config

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

- Modify `Podfile`:

```
pod 'ee-x/firebase-remote-config', :git => 'https://github.com/Senspark/ee-x'
```

### Storage

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

- Modify `Podfile`:

```
pod 'ee-x/firebase-storage', :git => 'https://github.com/Senspark/ee-x'
```