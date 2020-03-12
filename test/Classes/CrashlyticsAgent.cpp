//
//  CrashlyticsAgent.cpp
//  ee_x_test
//
//  Created by Zinge on 5/17/17.
//
//

#include "CrashlyticsAgent.hpp"

#include <ee/Crashlytics.hpp>

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
