//
//  AdjustConfig.cpp
//  Adjust
//
//  Created by eps on 8/19/20.
//

#include "ee/adjust/AdjustConfig.hpp"

namespace ee {
namespace adjust {
using Self = Config;

Self& Self::setToken(const std::string& token) {
    token_ = token;
    return *this;
}

Self& Self::setEnvironment(Environment environment) {
    environment_ = environment;
    return *this;
}

Self& Self::setLogLevel(LogLevel logLevel) {
    logLevel_ = logLevel;
    return *this;
}
} // namespace adjust
} // namespace ee
