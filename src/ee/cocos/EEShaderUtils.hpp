//
//  EEShader.hpp
//  ee-library
//
//  Created by Zinge on 6/18/16.
//
//

#ifndef EE_LIBRARY_SHADER_UTILS_HPP
#define EE_LIBRARY_SHADER_UTILS_HPP

#include "EECocos2dxFwd.hpp"

namespace ee {
/// Creates a shear matrix.
/// https://en.wikipedia.org/wiki/Shear_matrix
cocos2d::Mat4 createShearZMatrix(float dx, float dy);

void transformRGB(const cocos2d::Mat4& mat, float red, float green, float blue,
                  float& transformedRed, float& transformedGreen,
                  float& transformedBlue);

/// | c 0 0 0 |
/// | 0 c 0 0 |
/// | 0 0 c 0 |
/// | t t t 1 |
/// t = (1 - c) / 2.
cocos2d::Mat4 createContrastMatrix(float contrast);

/// | r 0 0 0 |
/// | 0 g 0 0 |
/// | 0 0 b 0 |
/// | x y z 1 |
/// x = (1 - r) / 2
/// y = (1 - g) / 2
/// z = (1 - b) / 2
cocos2d::Mat4 createContrastMatrix(float red, float green, float blue);

/// | r + s   s       s       0
/// | g       g + s   s       0
/// | b       b       b + s   0
/// | 0       0       0       1
/// r = (1 - s) * 0.3086
/// g = (1 - s) * 0.6094
/// b = (1 - s) * 0.0820
cocos2d::Mat4 createSaturationMatrix(float saturation);

/// | 1 0 0 0 |
/// | 0 1 0 0 |
/// | 0 0 1 0 |
/// | b b b 1 |
cocos2d::Mat4 createBrightnessMatrix(float brightness);

/// | 1 0 0 0 |
/// | 0 1 0 0 |
/// | 0 0 1 0 |
/// | r g b 1 |
cocos2d::Mat4 createBrightnessMatrix(float red, float green, float blue);

cocos2d::Mat4 createHueMatrix(float degree);

cocos2d::GLProgramState* createHsvProgramState();

/// Creates a horizontal blur GL program.
/// @param[in] width The width of the texture in pixels.
/// @param[in] blurRadius The blur range in pixels.
/// @param[in] useLinearSampling Whether or not to use linear sampling,
/// effective only when blurRadius is even.
/// @param[in] sigma How strong the blur is.
cocos2d::GLProgram* createHorizontalBlurProgram(std::size_t width,
                                                std::size_t blurRadius,
                                                bool useLinearSampling,
                                                float sigma);

/// Creates a vertical blur GL program.
/// @param[in] height The height of the texture in pixels.
/// @param[in] blurRadius The blur range in pixels.
/// @param[in] useLinearSampling Whether or not to use linear sampling,
/// effective only when blurRadius is even.
/// @param[in] sigma How strong the blur is.
cocos2d::GLProgram* createVerticalBlurProgram(std::size_t height,
                                              std::size_t blurRadius,
                                              bool useLinearSampling,
                                              float sigma);
} // namespace ee

#endif /* EE_LIBRARY_SHADER_UTILS_HPP */
