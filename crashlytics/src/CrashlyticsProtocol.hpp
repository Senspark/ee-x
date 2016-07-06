//
//  CrashlyticsProtocol.hpp
//  ee-crashlytics
//
//  Created by Zinge on 7/4/16.
//
//

#ifndef EE_CRASHLYTICS_CRASHLYTICS_PROTOCOL_HPP_
#define EE_CRASHLYTICS_CRASHLYTICS_PROTOCOL_HPP_

#include <memory>
#include <string>

namespace ee {
namespace core {
class PluginProtocol;
} // namespace core

namespace crashlytics {
class LogLevel {
public:
    static const LogLevel Verbose;
    static const LogLevel Debug;
    static const LogLevel Info;
    static const LogLevel Warn;
    static const LogLevel Error;
    static const LogLevel Assert;

private:
    friend class CrashlyticsProtocol;

    // Log level priorities are specified here:
    // https://developer.android.com/reference/android/util/Log.html
    explicit LogLevel(int priority, const std::string& desc);

    int priority_;
    std::string desc_;
};

class CrashlyticsProtocol final {
public:
    CrashlyticsProtocol();
    ~CrashlyticsProtocol();

    void causeCrash() const;

    void causeException() const;

    void log(const LogLevel& level, const std::string& tag,
             const std::string& message) const;

private:
    std::unique_ptr<core::PluginProtocol> protocol_;
};
} // namespace crashlytics
} // namespace ee

#endif /* CrashlyticsProtocol_hpp */
