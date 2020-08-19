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

private:
    friend Bridge;

    std::string token_;
    Environment environment_;
    LogLevel logLevel_;
};
} // namespace adjust
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_ADJUST_CONFIG_HPP */
