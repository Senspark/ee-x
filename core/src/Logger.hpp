//
//  PluginUtils.hpp
//  ee-core
//
//  Created by Zinge on 7/6/16.
//
//

#ifndef EE_CORE_LOGGER_HPP_
#define EE_CORE_LOGGER_HPP_

#include <sstream>
#include <string>

namespace ee {
namespace core {
class LogLevel;

class Logger {
public:
    static void log(const LogLevel& level, const std::string& tag,
                    const std::string& message);

    explicit Logger(const std::string& tag);

    void setEnabled(bool enabled) noexcept;

    bool isEnabled() const noexcept;

    void log(const LogLevel& level, const std::string& message) const;

    void debug(const std::string& message) const;

    void info(const std::string& message) const;

    void warn(const std::string& message) const;

    void error(const std::string& message) const;

    template <class... Args>
    void log(const LogLevel& level, Args&&... args) const {
        log(level, toString(std::forward<Args>(args)...));
    }

    template <class... Args> void debug(Args&&... args) const {
        debug(toString(std::forward<Args>(args)...));
    }

    template <class... Args> void info(Args&&... args) const {
        info(toString(std::forward<Args>(args)...));
    }

    template <class... Args> void warn(Args&&... args) const {
        warn(toString(std::forward<Args>(args)...));
    }

    template <class... Args> void error(Args&&... args) const {
        error(toString(std::forward<Args>(args)...));
    }

private:
    template <class... Args> static std::string toString(Args&&... args) {
        static std::stringstream ss;
        ss.clear();
        ss.str(std::string{});
        (ss << ... << std::forward<Args>(args));
        return ss.str();
    }

    bool enabled_;

    std::string tag_;
};
} // namespace core
} // namespace ee

#endif /* EE_CORE_LOGGER_HPP_ */
