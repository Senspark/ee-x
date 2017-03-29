//
//  CrashlyticsProtocol.hpp
//  ee-crashlytics
//
//  Created by Zinge on 7/4/16.
//
//

#ifndef EE_X_CRASHLYTICS_HPP_
#define EE_X_CRASHLYTICS_HPP_

#include <memory>
#include <string>

namespace ee {
struct LogLevel;

class Crashlytics final {
public:
    /// Causes a crash.
    void causeCrash() const;

    /// Causes an exception.
    void causeException() const;

    void setLogLevel(const LogLevel& level) const;

    void log(const LogLevel& level, const std::string& tag,
             const std::string& message) const;

    void setString(const std::string& key, const std::string& value) const;

    void setBool(const std::string& key, bool value) const;

    void setInt(const std::string& key, int value) const;

    void setUserIdentifier(const std::string& identifier) const;

    void setUserName(const std::string& name) const;

    void setUserEmail(const std::string& email) const;
};
} // namespace ee

#endif /* EE_X_CRASHLYTICS_HPP_ */
