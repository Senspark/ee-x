//
//  CrashlyticsProtocol.hpp
//  ee-crashlytics
//
//  Created by Zinge on 7/4/16.
//
//

#ifndef EE_X_CRASHLYTICS_CRASHLYTICS_BRIDGE_HPP_
#define EE_X_CRASHLYTICS_CRASHLYTICS_BRIDGE_HPP_

#include <string>

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

private:
    IMessageBridge& bridge_;
};
} // namespace crashlytics
} // namespace ee

#endif /* EE_X_CRASHLYTICS_CRASHLYTICS_BRIDGE_HPP_ */
