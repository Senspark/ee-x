#ifndef EE_X_I_LOGGER_HPP
#define EE_X_I_LOGGER_HPP

#ifdef __cplusplus

#include <cstdarg>
#include <string>

#include "ee/core/CoreFwd.hpp"

namespace ee {
namespace core {
class ILogger {
public:
    virtual ~ILogger() = default;

    virtual void verbose(std::string formatString, ...) = 0;
    virtual void debug(std::string formatString, ...) = 0;
    virtual void info(std::string formatString, ...) = 0;
    virtual void warn(std::string formatString, ...) = 0;
    virtual void error(std::string formatString, ...) = 0;
    virtual void wtf(std::string formatString, ...) = 0;
};
} // namespace core
} // namespace ee

#endif // __cplusplus

#endif // EE_X_I_LOGGER_HPP