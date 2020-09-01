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

#include "ee/AdjustFwd.hpp"

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

    Self& setAppSecret(std::uint64_t secretId, //
                       std::uint64_t info1, std::uint64_t info2,
                       std::uint64_t info3, std::uint64_t info4);

private:
    friend Bridge;

    std::string token_;
    Environment environment_;
    LogLevel logLevel_;
    bool eventBufferingEnabled_;
    bool useAppSecret_;
    std::uint64_t secretId_;
    std::uint64_t info1_;
    std::uint64_t info2_;
    std::uint64_t info3_;
    std::uint64_t info4_;
};
} // namespace adjust
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_ADJUST_CONFIG_HPP */
