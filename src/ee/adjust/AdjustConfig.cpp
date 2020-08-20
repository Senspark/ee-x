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

Self& Self::setAppSecret(std::uint64_t secretId, //
                         std::uint64_t info1, std::uint64_t info2,
                         std::uint64_t info3, std::uint64_t info4) {
    useAppSecret_ = true;
    secretId_ = secretId;
    info1_ = info1;
    info2_ = info2;
    info3_ = info3;
    info4_ = info4;
    return *this;
}
} // namespace adjust
} // namespace ee
