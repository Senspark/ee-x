//
//  CrashlyticsProtocol.hpp
//  ee-crashlytics
//
//  Created by Zinge on 7/4/16.
//
//

#ifndef EE_X_CRASHLYTICS_CRASHLYTICS_BRIDGE_HPP_
#define EE_X_CRASHLYTICS_CRASHLYTICS_BRIDGE_HPP_

#include <memory>
#include <string>
#include <unordered_map>

namespace ee {
namespace core {
struct LogLevel;
} // namespace core

namespace crashlytics {
class Crashlytics final {
public:
    /// Causes a crash.
    void causeCrash() const;

    /// Causes an exception.
    void causeException() const;

    void setLogLevel(const core::LogLevel& level) const;

    void log(const core::LogLevel& level, const std::string& tag,
             const std::string& message) const;

    void setString(const std::string& key, const std::string& value) const;

    void setBool(const std::string& key, bool value) const;

    void setInt(const std::string& key, int value) const;

    void setUserIdentifier(const std::string& identifier) const;

    void setUserName(const std::string& name) const;

    void setUserEmail(const std::string& email) const;
    
    void trackLevelStart(const std::string& name, const std::unordered_map<std::string, std::string>& attrs);
    
    void trackLevelEnd(const std::string& name, int score, bool success, const std::unordered_map<std::string, std::string>& attrs);
    
    void trackPurchase(float price, const std::string& currency, bool success, const std::string& itemName, const std::string& itemType, const std::string& itemId, const std::unordered_map<std::string, std::string>& attrs);
    
    void trackCustomEvent(const std::string& name, const std::unordered_map<std::string, std::string>& attrs);
    
    void trackInvite(const std::string& method, const std::unordered_map<std::string, std::string>& attrs);
};
} // namespace crashlytics
} // namespace ee

#endif /* EE_X_CRASHLYTICS_CRASHLYTICS_BRIDGE_HPP_ */
