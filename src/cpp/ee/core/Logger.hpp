//
//  LoggerInterface.hpp
//  ee_x
//
//  Created by Zinge on 11/1/17.
//
//

#ifndef EE_X_LOGGER_HPP
#define EE_X_LOGGER_HPP

#ifdef __cplusplus

#include <functional>

#include "ee/core/ILogger.hpp"

namespace ee {
namespace core {
class Logger final : public ILogger {
private:
    using Self = Logger;

public:
    using LogCallback =
        std::function<void(const LogLevel& level, const std::string& tag,
                           const std::string& message)>;

    /// Gets the logger used by the library.
    static Self& getSystemLogger();

    /// Sets the logger used by the library.
    static void setSystemLogger(const Self& logger);

    /// Constructs an empty logger (doesn't log anything).
    static Self nullLogger();

    /// Constructs a logger with the specified tag and a default callback
    /// (Android Logcat or iOS/macOS console).
    explicit Logger(const std::string& tag);

    /// Constructs a logger with the specified tag and callback.
    explicit Logger(const std::string& tag, const LogCallback& callback);

    virtual ~Logger() = default;

    void setEnabled(bool enabled);
    void log(const LogLevel& level, std::string formatString, ...);

    virtual void verbose(std::string formatString, ...) override;
    virtual void debug(std::string formatString, ...) override;
    virtual void info(std::string formatString, ...) override;
    virtual void warn(std::string formatString, ...) override;
    virtual void error(std::string formatString, ...) override;
    virtual void wtf(std::string formatString, ...) override;

protected:
    void log(const LogLevel& level, std::string formatString,
             std::va_list args);

private:
    bool enabled_;
    std::string tag_;
    LogCallback callback_;
};
} // namespace core
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_LOGGER_HPP */
