//
//  CrashlyticsProtocol.hpp
//  ee-crashlytics
//
//  Created by Zinge on 7/4/16.
//
//

#ifndef EE_CRASHLYTICS_CRASHLYTICS_PROTOCOL_HPP_
#define EE_CRASHLYTICS_CRASHLYTICS_PROTOCOL_HPP_

#include <PluginProtocol.hpp>

namespace ee {
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

    // Log level priorties are specified here:
    // https://developer.android.com/reference/android/util/Log.html
    explicit LogLevel(int priority, const std::string& desc);

    int priority_;
    std::string desc_;
};

class CrashlyticsProtocol : public core::PluginProtocol {
public:
    CrashlyticsProtocol();
    
    void causeCrash() const;

    void causeException() const;

    void log(const LogLevel& level, const std::string& tag,
             const std::string& message) const;
};
} // namespace crashlytics
} // namespace ee

#endif /* CrashlyticsProtocol_hpp */
