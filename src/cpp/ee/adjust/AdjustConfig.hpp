//
//  AdjustConfig.hpp
//  Adjust
//
//  Created by eps on 8/19/20.
//

#ifndef EE_X_ADJUST_CONFIG_HPP
#define EE_X_ADJUST_CONFIG_HPP

#ifdef __cplusplus

#include <string>

#include "ee/adjust/AdjustFwd.hpp"

namespace ee {
namespace adjust {
class Config final {
private:
    using Self = Config;

public:
    Self& setToken(const std::string& token);
    Self& setEnvironment(Environment environment);
    Self& setLogLevel(LogLevel logLevel);

    /// If your app makes heavy use of event tracking, you might want to delay
    /// some network requests in order to send them in one batch every minute.
    Self& setEventBufferingEnabled(bool enabled);

private:
    friend Bridge;

    std::string token_;
    Environment environment_;
    LogLevel logLevel_;
    bool eventBufferingEnabled_;
};
} // namespace adjust
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_ADJUST_CONFIG_HPP */
