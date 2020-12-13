//
//  EEImage.hpp
//  EE Library
//
//  Created by enrevol on 10/20/15.
//
//

#ifndef EE_LIBRARY_IMAGE_HPP_
#define EE_LIBRARY_IMAGE_HPP_

#ifdef __cplusplus

#include <cstdint>

#include "ee/cocos/CocosFwd.hpp"

namespace ee {
namespace image {
using SizeType = std::uint_fast32_t;
using PixelType = std::uint32_t;
using ChannelType = std::uint8_t;

static_assert(sizeof(ChannelType) * 4 == sizeof(PixelType));

/// Box blur.
/// https://en.wikipedia.org/wiki/Box_blur
void boxBlur2D(cocos2d::Image* image, SizeType range);

/// Box blur with two separate passes: a horizontal pass and a vertical pass.
void boxBlur1D(cocos2d::Image* image, SizeType range, SizeType iterations = 1);

/// Tent blur.
///
/// Equivalent to two iterations of box blur.
void tentBlur1D(cocos2d::Image* image, SizeType range, SizeType iterations = 1);

/// Cache oblivious matrix transposition.
void transpose(PixelType* src, PixelType* dst, SizeType srcWidth,
               SizeType srcHeight) noexcept;
} // namespace image.

using image::boxBlur1D;
using image::boxBlur2D;

using image::tentBlur1D;
} // namespace ee

#endif // __cplusplus

#endif /* EE_LIBRARY_IMAGE_HPP_ */
