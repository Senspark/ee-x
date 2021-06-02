#ifndef EE_X_SERVICES_FWD_HPP
#define EE_X_SERVICES_FWD_HPP

#ifdef __cplusplus

#include <ee/cocos/CocosFwd.hpp>

namespace ee {
namespace services {
class AdsConfig;
class LazyBannerAd;
class LazyFullScreenAd;
class GenericBannerAd;
class GenericFullScreenAd;
class IAdsManager;
class DefaultAdsManager;
} // namespace services

using services::DefaultAdsManager;
using services::IAdsManager;
} // namespace ee

#endif // __cplusplus

#endif // EE_X_SERVICES_FWD_HPP
