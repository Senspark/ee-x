//
//  PluginUtils.hpp
//  ee-core
//
//  Created by Zinge on 7/6/16.
//
//

#ifndef EE_X_CORE_LOGGER_HPP_
#define EE_X_CORE_LOGGER_HPP_

#include <sstream>
#include <string>

namespace ee {
namespace core {
struct LogLevel;

class Logger {
public:
    template <class... Args>
    static void log(const LogLevel& level, const std::string& tag,
                    Args&&... args) {
        log0(level, tag, toString(std::forward<Args>(args)...));
    }

    explicit Logger(const std::string& tag);

    void setEnabled(bool enabled) noexcept;

    bool isEnabled() const noexcept;

    template <class... Args>
    void log(const LogLevel& level, Args&&... args) const {
        if (isEnabled()) {
            log0(level, toString(std::forward<Args>(args)...));
        }
    }

    template <class... Args> void debug(Args&&... args) const {
        if (isEnabled()) {
            debug0(toString(std::forward<Args>(args)...));
        }
    }

    template <class... Args> void info(Args&&... args) const {
        if (isEnabled()) {
            info0(toString(std::forward<Args>(args)...));
        }
    }

    template <class... Args> void warn(Args&&... args) const {
        if (isEnabled()) {
            warn0(toString(std::forward<Args>(args)...));
        }
    }

    template <class... Args> void error(Args&&... args) const {
        if (isEnabled()) {
            error0(toString(std::forward<Args>(args)...));
        }
    }

private:
    static void log0(const LogLevel& level, const std::string& tag,
                     const std::string& message);

    void log0(const LogLevel& level, const std::string& message) const;

    void debug0(const std::string& message) const;

    void info0(const std::string& message) const;

    void warn0(const std::string& message) const;

    void error0(const std::string& message) const;

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

using core::Logger;
} // namespace ee

#endif /* EE_X_CORE_LOGGER_HPP_ */
