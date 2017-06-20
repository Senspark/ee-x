//
//  AppDelegate.cpp
//  ee_x_test
//
//  Created by Zinge on 5/17/17.
//
//

#include "AppDelegate.hpp"
#include "CrashlyticsAgent.hpp"
#include "NotificationAgent.hpp"

#include <ee/FacebookAds.hpp>
#include <ee/Macro.hpp>

AppDelegate* AppDelegate::getInstance() {
    static AppDelegate sharedInstance;
    return &sharedInstance;
}

bool AppDelegate::applicationDidFinishLaunching() {

    CrashlyticsAgent::getInstance()->initialize();
    CrashlyticsAgent::getInstance()->logDebug("debug_message");
    CrashlyticsAgent::getInstance()->logInfo("info_message");
    CrashlyticsAgent::getInstance()->logError("error_message");
    CrashlyticsAgent::getInstance()->trackLevelStart("level_test");
    CrashlyticsAgent::getInstance()->trackLevelEnd("level_test", 100, true);
    CrashlyticsAgent::getInstance()->trackPurchase(100.f, "USD", true, "Item Test", "Test Type", "TEST_ID");
    
    CrashlyticsAgent::CustomAttributesType attrs;
    attrs["title"] = "Beautiful in white";
    attrs["singer"] = "Westlife";
    CrashlyticsAgent::getInstance()->trackCustomEvent("PlaySong", attrs);
    CrashlyticsAgent::getInstance()->trackInvite("Twitter");

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
#ifndef EE_X_DESKTOP
    NotificationAgent::getInstance()->scheduleAll();
#endif // EE_X_DESKTOP
}

void AppDelegate::applicationWillEnterForeground() {
#ifndef EE_X_DESKTOP
    NotificationAgent::getInstance()->unscheduleAll();
#endif // EE_X_DESKTOP
}
