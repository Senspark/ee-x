//
//  CrashlyticsAgent.hpp
//  ee_x_test
//
//  Created by Zinge on 5/17/17.
//
//

#ifndef EE_X_TEST_CRASHLYTICS_AGENT_HPP_
#define EE_X_TEST_CRASHLYTICS_AGENT_HPP_

#include <memory> // std::unique_ptr.
#include <string> // std::string.
#include <unordered_map> //std::unordered_map

#include <ee/CrashlyticsFwd.hpp>

class CrashlyticsAgent {
public:
    static CrashlyticsAgent* getInstance();
    
    using CustomAttributesType = std::unordered_map<std::string, std::string>;
    
    static const CustomAttributesType EmptyAttributes;
    
    void initialize();
    void causeCrash();
    void causeException();
    void logDebug(const std::string& message);
    void logInfo(const std::string& message);
    void logError(const std::string& message);
    
    void trackLevelStart(const std::string& name, const CustomAttributesType& attrs = EmptyAttributes);
    void trackLevelEnd(const std::string& name, int score, bool success, const CustomAttributesType& attrs = EmptyAttributes);
    void trackPurchase(float price, const std::string& currency, bool success, const std::string& itemName, const std::string& itemType, const std::string& itemId, const CustomAttributesType& attrs = EmptyAttributes);
    void trackCustomEvent(const std::string& name, const CustomAttributesType& attrs = EmptyAttributes);
    void trackInvite(const std::string& method, const CustomAttributesType& attrs = EmptyAttributes);
private:
    std::unique_ptr<ee::Crashlytics> protocol_;
};

#endif /* EE_X_TEST_CRASHLYTICS_AGENT_HPP_ */
