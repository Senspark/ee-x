#ifndef EE_X_IRON_SOURCE_BANNER_AD_SIZE_HPP
#define EE_X_IRON_SOURCE_BANNER_AD_SIZE_HPP

#ifdef __cplusplus

#include "ee/iron_source/IronSourceFwd.hpp"

namespace ee {
namespace iron_source {
enum class BannerAdSize {
    /// 320x50.
    Banner,

    /// 320x90.
    Large,

    /// 300x250.
    Rectangle,
};
} // namespace iron_source
} // namespace ee

#endif // __cplusplus

#endif // EE_X_IRON_SOURCE_BANNER_AD_SIZE_HPP