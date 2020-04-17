//
//  CrashlyticsAgent.cpp
//  ee_x_test
//
//  Created by Zinge on 5/17/17.
//
//

#include "CrashlyticsAgent.hpp"

#include <cocos2d.h>

#include <ee/Firebase.hpp>

CrashlyticsAgent* CrashlyticsAgent::getInstance() {
    static CrashlyticsAgent sharedInstance;
    return &sharedInstance;
}

void CrashlyticsAgent::initialize() {
    plugin_ = std::make_unique<ee::FirebaseCrashlytics>();
}

void CrashlyticsAgent::log(const std::string& message) {
    plugin_->log(message);
}

void CrashlyticsAgent::logDebug(const std::string& message) {
    log(message);
    CCLOG("%s", message.c_str());
}

void CrashlyticsAgent::logInfo(const std::string& message) {
    log(message);
    cocos2d::log("%s", message.c_str());
}
