//
//  EETentBlur1D.cpp
//  EE Library
//
//  Created by enrevol on 10/22/15.
//
//

#include <cassert>
#include <cstring>

#include "EEImage.hpp"

#include <platform/CCImage.h>

NS_EE_BEGIN
namespace image {
NS_ANONYMOUS_BEGIN
void internalTentBlur1D(ChannelType* pixels, SizeType width, SizeType height,
                        SizeType range, SizeType iterations) {
    auto kernelSize = range * 2 + 1;
    assert(kernelSize < width && kernelSize < height);

    constexpr SizeType Bits = 24;
    SizeType numerators[kernelSize + 1];
    for (SizeType size = 1, weight = 0; size <= kernelSize; ++size) {
        weight += (size <= range) ? size : (kernelSize - size + 1);
        numerators[size] = (1 << Bits) / weight;
    }

    auto prefixSumR = new SizeType[width + 1];
    prefixSumR[0] = 0;
    auto prefixSumG = new SizeType[width + 1];
    prefixSumG[0] = 0;
    auto prefixSumB = new SizeType[width + 1];
    prefixSumB[0] = 0;
    auto buffer = new ChannelType[width * 4];

    auto newPixel = buffer;
    auto sumInR = prefixSumR;
    auto sumInG = prefixSumG;
    auto sumInB = prefixSumB;
    auto sumMidR = prefixSumR;
    auto sumMidG = prefixSumG;
    auto sumMidB = prefixSumB;
    auto sumOutR = prefixSumR;
    auto sumOutG = prefixSumG;
    auto sumOutB = prefixSumB;
    SizeType sumR = 0;
    SizeType sumG = 0;
    SizeType sumB = 0;

    auto inPixel = pixels;

    auto shiftSumIn = [&]() noexcept {
        ++sumInR;
        ++sumInG;
        ++sumInB;
    };
    auto shiftSumMid = [&]() noexcept {
        ++sumMidR;
        ++sumMidG;
        ++sumMidB;
    };
    auto shiftSumOut = [&]() noexcept {
        ++sumOutR;
        ++sumOutG;
        ++sumOutB;
    };
    auto addPixel = [&]() noexcept {
        *(sumInR + 1) = *sumInR + *inPixel++;
        *(sumInG + 1) = *sumInG + *inPixel++;
        *(sumInB + 1) = *sumInB + *inPixel++;
        inPixel++;
    };
    auto addSum = [&]() noexcept {
        sumR += *sumInR - *sumMidR;
        sumG += *sumInG - *sumMidG;
        sumB += *sumInB - *sumMidB;
    };
    auto removeSum = [&]() noexcept {
        sumR -= *sumMidR - *sumOutR;
        sumG -= *sumMidG - *sumOutG;
        sumB -= *sumMidB - *sumOutB;
    };
    auto updatePixel = [&](SizeType size) noexcept {
        *newPixel++ =
            static_cast<ChannelType>((sumR * numerators[size]) >> Bits);
        *newPixel++ =
            static_cast<ChannelType>((sumG * numerators[size]) >> Bits);
        *newPixel++ =
            static_cast<ChannelType>((sumB * numerators[size]) >> Bits);
        *newPixel++ = std::numeric_limits<ChannelType>::max();
    };

    for (SizeType row = 0; row < height; ++row) {
        for (SizeType iteration = 0; iteration < iterations; ++iteration) {
            sumR = sumG = sumB = 0;
            sumInR = sumMidR = sumOutR = prefixSumR;
            sumInG = sumMidG = sumOutG = prefixSumG;
            sumInB = sumMidB = sumOutB = prefixSumB;
            inPixel = pixels;
            newPixel = buffer;
            for (SizeType col = 0; col < range; ++col) {
                addPixel();
                shiftSumIn();
                addSum();
            }
            for (SizeType col = range; col <= range + range; ++col) {
                addPixel();
                shiftSumIn();
                addSum();
                removeSum();
                shiftSumMid();
                updatePixel(col + 1);
            }
            for (SizeType col = range + range + 1; col < width; ++col) {
                addPixel();
                shiftSumIn();
                addSum();
                removeSum();
                shiftSumMid();
                shiftSumOut();
                updatePixel(kernelSize);
            }
            for (SizeType col = 0; col < range; ++col) {
                addSum();
                removeSum();
                shiftSumMid();
                shiftSumOut();
                updatePixel(kernelSize - col - 1);
            }
            std::memcpy(pixels, buffer, width * 4);
        }
        pixels = inPixel;
    }

    delete[] buffer;
    delete[] prefixSumR;
    delete[] prefixSumG;
    delete[] prefixSumB;
}
NS_ANONYMOUS_END

void tentBlur1D(cocos2d::Image* image, SizeType range, SizeType iterations) {
    auto pixels = image->getData();
    auto width = static_cast<SizeType>(image->getWidth());
    auto height = static_cast<SizeType>(image->getHeight());
    auto buffer = new ChannelType[width * height * 4];
    auto pixelsPtr = reinterpret_cast<PixelType*>(pixels);
    auto bufferPtr = reinterpret_cast<PixelType*>(buffer);

    // Horizonal pass.
    internalTentBlur1D(pixels, width, height, range, iterations);
    transpose(pixelsPtr, bufferPtr, width, height);

    // Vertical pass.
    internalTentBlur1D(buffer, height, width, range, iterations);
    transpose(bufferPtr, pixelsPtr, height, width);

    delete[] buffer;
}
} // namespace image.
NS_EE_END