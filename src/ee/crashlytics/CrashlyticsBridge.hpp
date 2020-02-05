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

#include "ee/CrashlyticsFwd.hpp"

namespace ee {
namespace crashlytics {
class Bridge final {
public:
    Bridge();
    
    /// Causes a crash.
    void causeCrash() const;

    /// Causes an exception.
    void causeException() const;

    /// Sets the default log level.
    /// @param[in] level The desired log level.
    void setLogLevel(const LogLevel& level) const;

    /// Logs a message.
    /// @param[in] level The message's log level.
    /// @param[in] tag The message's tag.
    /// @param[in] message The message's content.
    void log(const LogLevel& level, const std::string& tag,
             const std::string& message) const;

    /// Sets a string value for user properties.
    void setString(const std::string& key, const std::string& value) const;

    /// Sets a bool value for user properties.
    void setBool(const std::string& key, bool value) const;

    /// Sets an int value for user properties.
    void setInt(const std::string& key, int value) const;

    /// Sets the user's identifier.
    void setUserIdentifier(const std::string& identifier) const;

    /// Sets the user's name.
    void setUserName(const std::string& name) const;

    /// Sets the user's email.
    void setUserEmail(const std::string& email) const;

    void
    trackLevelStart(const std::string& name,
                    const std::unordered_map<std::string, std::string>& attrs);

    void
    trackLevelEnd(const std::string& name, int score, bool success,
                  const std::unordered_map<std::string, std::string>& attrs);

    void
    trackPurchase(float price, const std::string& currency, bool success,
                  const std::string& itemName, const std::string& itemType,
                  const std::string& itemId,
                  const std::unordered_map<std::string, std::string>& attrs);

    void
    trackCustomEvent(const std::string& name,
                     const std::unordered_map<std::string, std::string>& attrs);

    void trackInvite(const std::string& method,
                     const std::unordered_map<std::string, std::string>& attrs);
    
private:
    IMessageBridge& bridge_;
};
} // namespace crashlytics
} // namespace ee

#endif /* EE_X_CRASHLYTICS_CRASHLYTICS_BRIDGE_HPP_ */
