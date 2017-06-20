//
//  CrashlyticsAgent.cpp
//  ee_x_test
//
//  Created by Zinge on 5/17/17.
//
//

#include "CrashlyticsAgent.hpp"

#include <ee/Crashlytics.hpp>
#include <ee/Core.hpp>

const CrashlyticsAgent::CustomAttributesType CrashlyticsAgent::EmptyAttributes = std::unordered_map<std::string, std::string>();

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

void CrashlyticsAgent::trackLevelStart(const std::string &name, const CustomAttributesType& attrs) {
    protocol_->trackLevelStart(name, attrs);
}

void CrashlyticsAgent::trackLevelEnd(const std::string &name, int score, bool success, const CustomAttributesType& attrs) {
    protocol_->trackLevelEnd(name, score, success, attrs);
}

void CrashlyticsAgent::trackPurchase(float price, const std::string &currency, bool success, const std::string &itemName, const std::string &itemType, const std::string &itemId, const CustomAttributesType& attrs) {
    protocol_->trackPurchase(price, currency, success, itemName, itemType, itemId, attrs);
}

void CrashlyticsAgent::trackCustomEvent(const std::string &name, const CustomAttributesType& attrs) {
    protocol_->trackCustomEvent(name, attrs);
}

void CrashlyticsAgent::trackInvite(const std::string &method, const CustomAttributesType& attrs) {
    protocol_->trackInvite(method, attrs);
}
