//
//  EEShader.cpp
//  ee-library
//
//  Created by Zinge on 6/18/16.
//
//

#include <iomanip>

#include "EEShaderManager.hpp"
#include "EEShaderUtils.hpp"
#include "EEUtils.hpp"

#include <base/CCDirector.h>
#include <base/CCEventDispatcher.h>
#include <base/CCEventListenerCustom.h>
#include <base/CCEventType.h>
#include <base/ccUTF8.h> // cocos2d::StringUtils::format (cocos2d-x 3.15)
#include <deprecated/CCString.h> // cocos2d::StringUtils::format (cocos2d-x 3.10)
#include <renderer/CCGLProgram.h>
#include <renderer/CCGLProgramCache.h>

namespace ee {
namespace constants {
constexpr const float rwgt = 0.3086f;
constexpr const float gwgt = 0.6094f;
constexpr const float bwgt = 0.0820f;
} // namespace constants

cocos2d::Mat4 createShearZMatrix(float dx, float dy) {
    auto mat = cocos2d::Mat4::IDENTITY;
    mat.m[2] = dx;
    mat.m[6] = dy;
    return mat;
}

void transformRGB(const cocos2d::Mat4& mat, float red, float green, float blue,
                  float& transformedRed, float& transformedGreen,
                  float& transformedBlue) {
    float r = red * mat.m[0] + green * mat.m[4] + blue * mat.m[8] + mat.m[12];
    float g = red * mat.m[1] + green * mat.m[5] + blue * mat.m[9] + mat.m[13];
    float b = red * mat.m[2] + green * mat.m[6] + blue * mat.m[10] + mat.m[14];

    transformedRed = r;
    transformedGreen = g;
    transformedBlue = b;
}

cocos2d::Mat4 createContrastMatrix(float contrast) {
    return createContrastMatrix(contrast, contrast, contrast);
}

cocos2d::Mat4 createContrastMatrix(float red, float green, float blue) {
    auto mat =
        createBrightnessMatrix((1 - red) / 2, (1 - green) / 2, (1 - blue) / 2);
    mat.m[0] = red;
    mat.m[5] = green;
    mat.m[10] = blue;
    return mat;
}

cocos2d::Mat4 createSaturationMatrix(float saturation) {
    auto mat = cocos2d::Mat4::ZERO;

    mat.m[0] = (1.0f - saturation) * constants::rwgt + saturation;
    mat.m[1] = (1.0f - saturation) * constants::rwgt;
    mat.m[2] = (1.0f - saturation) * constants::rwgt;

    mat.m[4] = (1.0f - saturation) * constants::gwgt;
    mat.m[5] = (1.0f - saturation) * constants::gwgt + saturation;
    mat.m[6] = (1.0f - saturation) * constants::gwgt;

    mat.m[8] = (1.0f - saturation) * constants::bwgt;
    mat.m[9] = (1.0f - saturation) * constants::bwgt;
    mat.m[10] = (1.0f - saturation) * constants::bwgt + saturation;

    mat.m[15] = 1.0f;

    return mat;
}

cocos2d::Mat4 createBrightnessMatrix(float brightness) {
    return createBrightnessMatrix(brightness, brightness, brightness);
}

cocos2d::Mat4 createBrightnessMatrix(float red, float green, float blue) {
    cocos2d::Mat4 mat;
    cocos2d::Mat4::createTranslation(red, green, blue, &mat);
    return mat;
}

cocos2d::Mat4 createHueMatrix(float degree) {
    auto mat = cocos2d::Mat4::IDENTITY;

    cocos2d::Mat4 temp;

    // Rotate the grey vector into positive Z.
    // Sin = 1/sqrt(2).
    // Cos = 1/sqrt(2).
    cocos2d::Mat4::createRotationX(static_cast<float>(M_PI_4), &temp);
    cocos2d::Mat4::multiply(temp, mat, &mat);

    // Sin = -1/sqrt(3).
    // Cos = sqrt(2/3).
    cocos2d::Mat4::createRotationY(-0.615479709f, &temp);
    cocos2d::Mat4::multiply(temp, mat, &mat);

    // Shear the space to make the luminance plane horizontal.
    float lx, ly, lz;
    transformRGB(mat, constants::rwgt, constants::gwgt, constants::bwgt, lx, ly,
                 lz);

    float zsx = lx / lz;
    float zsy = ly / lz;
    temp = createShearZMatrix(zsx, zsy);
    cocos2d::Mat4::multiply(temp, mat, &mat);

    // Rotate the hue.
    float rad = CC_DEGREES_TO_RADIANS(degree);
    cocos2d::Mat4::createRotationZ(rad, &temp);
    cocos2d::Mat4::multiply(temp, mat, &mat);

    // Unshear the space to put the luminance plane back.
    temp = createShearZMatrix(-zsx, -zsy);
    cocos2d::Mat4::multiply(temp, mat, &mat);

    // Rotate the grey vector back into place.
    // Sin = 1/sqrt(3).
    // Cos = sqrt(2/3);
    cocos2d::Mat4::createRotationY(0.615479709f, &temp);
    cocos2d::Mat4::multiply(temp, mat, &mat);

    // Sin = -1/sqrt(2).
    // Cos = 1/sqrt(2).
    cocos2d::Mat4::createRotationX(static_cast<float>(-M_PI_4), &temp);
    cocos2d::Mat4::multiply(temp, mat, &mat);

    return mat;
}

namespace program {
constexpr auto hsv = "ee_hsv_program";
} // namespace program

#include "EEHsvShader.frag"
#include "EEHsvShader.vert"

cocos2d::GLProgramState* createHsvProgramState() {
    auto&& manager = ShaderManager::getInstance();
    if (not manager.isAdded(program::hsv)) {
        manager.addShader(program::hsv, ee_hsv_shader_vert, ee_hsv_shader_frag);
    }
    return manager.createProgramState(program::hsv);
}

namespace {
float gaussianFunction(float x, float sigma) {
    // 1/(sigma * sqrt(2 * pi) * exp(-(x/sigma)^2 / 2).
    constexpr float inv_sqrt_2pi = 0.39894228f;

    float k = x / sigma;
    return inv_sqrt_2pi / sigma * std::exp(-k * k * 0.5f);
}
} // namespace

std::string createBlurVertexShader(bool isVertical, std::size_t dimension,
                                   std::size_t blurRadius,
                                   bool useLinearSampling, float sigma);

std::string createBlurFragmentShader(std::size_t blurRadius,
                                     bool useLinearSampling, float sigma);

cocos2d::GLProgram* createHorizontalBlurProgram(std::size_t width,
                                                std::size_t blurRadius,
                                                bool useLinearSampling,
                                                float sigma) {
    auto&& manager = ShaderManager::getInstance();
    auto key = cocos2d::StringUtils::format(
        "ee_horizontal_blur_%zu_%zu_%d_%.6f", width, blurRadius,
        static_cast<int>(useLinearSampling), sigma);
    if (not manager.isAdded(key)) {
        auto vert = createBlurVertexShader(false, width, blurRadius,
                                           useLinearSampling, sigma);
        auto frag =
            createBlurFragmentShader(blurRadius, useLinearSampling, sigma);
        manager.addShader(key, vert, frag);
    }
    return manager.getProgram(key);
}

cocos2d::GLProgram* createVerticalBlurProgram(std::size_t height,
                                              std::size_t blurRadius,
                                              bool useLinearSampling,
                                              float sigma) {
    auto&& manager = ShaderManager::getInstance();
    auto key = cocos2d::StringUtils::format(
        "ee_vertical_blur_%zu_%zu_%d_%.6f", height, blurRadius,
        static_cast<int>(useLinearSampling), sigma);
    if (not manager.isAdded(key)) {
        auto vert = createBlurVertexShader(true, height, blurRadius,
                                           useLinearSampling, sigma);
        auto frag =
            createBlurFragmentShader(blurRadius, useLinearSampling, sigma);
        manager.addShader(key, vert, frag);
    }
    return manager.getProgram(key);
}

std::string createBlurVertexShader(bool isVertical, std::size_t dimension,
                                   std::size_t blurRadius,
                                   bool useLinearSampling, float sigma) {
    auto size = blurRadius + 1;
    std::vector<float> offsets;

    if (useLinearSampling && (blurRadius % 2 == 0)) {
        std::vector<float> weights(size);
        float sum = 0;
        for (std::size_t i = 0; i < size; ++i) {
            weights.at(i) = gaussianFunction(i, sigma);
            sum += weights.at(i);
            if (i > 0) {
                sum += weights.at(i);
            }
        }
        for (auto& w : weights) {
            w /= sum;
        }

        offsets.reserve(size / 2);
        for (std::size_t i = 1; i < size; i += 2) {
            offsets.push_back(
                (i * weights.at(i) + (i + 1) * weights.at(i + 1)) /
                (weights.at(i) + weights.at(i + 1)));
        }
        CC_ASSERT(offsets.size() == size / 2);
    } else {
        offsets.reserve(size - 1);
        for (std::size_t i = 1; i < size; ++i) {
            offsets.push_back(i);
        }
    }

    std::string body;
    constexpr auto f0 = "%13.10f / %zu.0";
    constexpr auto f1 =
        "    v_blurTexCoords[%zu] = v_texCoord + vec2(%s, %s);\n";

    std::size_t vertexIndex = 0;
    for (auto i = offsets.size() - 1; ~i; --i) {
        auto offset =
            cocos2d::StringUtils::format(f0, -offsets.at(i), dimension);
        body += cocos2d::StringUtils::format(
            f1, vertexIndex++, (isVertical ? "0.0" : offset).c_str(),
            (isVertical ? offset : "0.0").c_str());
    }

    for (std::size_t i = 0; i < offsets.size(); ++i) {
        auto offset =
            cocos2d::StringUtils::format(f0, offsets.at(i), dimension);
        body += cocos2d::StringUtils::format(
            f1, vertexIndex++, (isVertical ? "0.0" : offset).c_str(),
            (isVertical ? offset : "0.0").c_str());
    }

    auto blurTexCoords = offsets.size() * 2;

    constexpr auto f2 = R"(
#ifdef GL_ES
precision mediump float;
#endif // GL_ES
    
attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

#ifdef GL_ES
varying lowp vec4 v_fragmentColor;
varying mediump vec2 v_texCoord;
varying mediump vec2 v_blurTexCoords[%zu];
#else // GL_ES
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
varying vec2 v_blurTexCoords[%zu];
#endif // GL_ES

void main() {
    gl_Position = CC_PMatrix * a_position;
    v_fragmentColor = a_color;
    v_texCoord = a_texCoord;
%s
})";

    auto result = cocos2d::StringUtils::format(f2, blurTexCoords, blurTexCoords,
                                               body.c_str());
    LOG_FUNC_FORMAT(
        "vertical = %s radius = %zu sampling = %s sigma = %f frag = %s",
        isVertical ? "true" : "false", blurRadius,
        useLinearSampling ? "true" : "false", sigma, result.c_str());

    return result;
}

std::string createBlurFragmentShader(std::size_t blurRadius,
                                     bool useLinearSampling, float sigma) {
    auto size = blurRadius + 1;

    // Calculate Gaussian weights.
    std::vector<float> weights(size);
    float sum = 0;
    for (std::size_t i = 0; i < size; ++i) {
        weights.at(i) = gaussianFunction(i, sigma);
        sum += weights.at(i);
        if (i > 0) {
            sum += weights.at(i);
        }
    }
    for (auto& w : weights) {
        w /= sum;
    }

    if (useLinearSampling && (blurRadius % 2 == 0)) {
        // Use linear sampling.
        for (std::size_t i = 1, j = 1; i < size; i += 2) {
            weights.at(j++) = weights.at(i) + weights.at(i + 1);
        }
        weights.resize((size + 1) / 2);
    }

    std::string body;
    constexpr auto f0 = "    gl_FragColor += texture2D(CC_Texture0, "
                        "v_blurTexCoords[%zu]) * %.10f;\n";
    constexpr auto f1 =
        "    gl_FragColor += texture2D(CC_Texture0, v_texCoord) * %.10f;\n";

    std::size_t vertexIndex = 0;
    for (auto i = weights.size() - 1; i > 0; --i) {
        body += cocos2d::StringUtils::format(f0, vertexIndex++, weights.at(i));
    }
    body += cocos2d::StringUtils::format(f1, weights.at(0));
    for (std::size_t i = 1; i < weights.size(); ++i) {
        body += cocos2d::StringUtils::format(f0, vertexIndex++, weights.at(i));
    }

    constexpr auto f2 = R"(
#ifdef GL_ES
precision mediump float;
#endif // GL_ES
    
#ifdef GL_ES
varying lowp vec4 v_fragmentColor;
varying mediump vec2 v_texCoord;
varying mediump vec2 v_blurTexCoords[%zu];
#else // GL_ES
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
varying vec2 v_blurTexCoords[%zu];
#endif // GL_ES
    
void main() {
    gl_FragColor = vec4(0.0);
%s
    gl_FragColor = gl_FragColor * v_fragmentColor;
})";

    auto blurTexCoords = (weights.size() - 1) * 2;
    auto result = cocos2d::StringUtils::format(f2, blurTexCoords, blurTexCoords,
                                               body.c_str());

    LOG_FUNC_FORMAT("radius = %zu sampling = %s sigma = %f frag = %s",
                    blurRadius, useLinearSampling ? "true" : "false", sigma,
                    result.c_str());

    return result;
}
} // namespace ee
