//
//  EEGausianBlur.cpp
//  EE Library
//
//  Created by enrevol on 10/20/15.
//
//

#include <cassert>
#include <cstring>

#include "EEImage.hpp"

#include <platform/CCImage.h>

NS_EE_BEGIN
namespace image {
NS_ANONYMOUS_BEGIN
void internalBoxBlur1D(ChannelType* pixels, SizeType width, SizeType height,
                       SizeType range, SizeType iterations) {
    auto kernelSize = range * 2 + 1;
    assert(kernelSize < width && kernelSize < height);

    constexpr SizeType Bits = 24;
    SizeType numerators[kernelSize + 1];
    for (auto size = kernelSize; size > 0; --size) {
        numerators[size] = (1 << Bits) / size;
    }

    auto buffer = new ChannelType[width * 4];

    auto sumR = new SizeType[width + 1];
    sumR[0] = 0;
    auto sumG = new SizeType[width + 1];
    sumG[0] = 0;
    auto sumB = new SizeType[width + 1];
    sumB[0] = 0;

    auto sumInR = sumR;
    auto sumInG = sumG;
    auto sumInB = sumB;
    auto sumOutR = sumR;
    auto sumOutG = sumG;
    auto sumOutB = sumB;

    auto inPixel = pixels;
    auto newPixel = buffer;

    auto shiftSumIn = [&]() noexcept {
        ++sumInR;
        ++sumInG;
        ++sumInB;
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
    auto updatePixel = [&](SizeType size) noexcept {
        auto num = numerators[size];
        *newPixel++ =
            static_cast<ChannelType>(((*sumInR - *sumOutR) * num) >> Bits);
        *newPixel++ =
            static_cast<ChannelType>(((*sumInG - *sumOutG) * num) >> Bits);
        *newPixel++ =
            static_cast<ChannelType>(((*sumInB - *sumOutB) * num) >> Bits);
        *newPixel++ = std::numeric_limits<ChannelType>::max();
    };

    for (SizeType row = 0; row < height; ++row) {
        for (SizeType iteration = 0; iteration < iterations; ++iteration) {
            sumInR = sumOutR = sumR;
            sumInG = sumOutG = sumG;
            sumInB = sumOutB = sumB;
            inPixel = pixels;
            newPixel = buffer;
            for (SizeType col = 0; col < range; ++col) {
                addPixel();
                shiftSumIn();
            }
            for (SizeType col = range; col <= range + range; ++col) {
                addPixel();
                shiftSumIn();
                updatePixel(col + 1);
            }
            for (SizeType col = range + range + 1; col < width; ++col) {
                addPixel();
                shiftSumIn();
                shiftSumOut();
                updatePixel(kernelSize);
            }
            for (SizeType col = 0; col < range; ++col) {
                shiftSumOut();
                updatePixel(kernelSize - col - 1);
            }
            std::memcpy(pixels, buffer, width * 4);
        }
        pixels = inPixel;
    }

    delete[] buffer;
    delete[] sumR;
    delete[] sumG;
    delete[] sumB;
}
NS_ANONYMOUS_END

void boxBlur1D(cocos2d::Image* image, SizeType range, SizeType iterations) {
    auto pixels = image->getData();
    auto width = static_cast<SizeType>(image->getWidth());
    auto height = static_cast<SizeType>(image->getHeight());
    auto buffer = new ChannelType[width * height * 4];
    auto pixelsPtr = reinterpret_cast<PixelType*>(pixels);
    auto bufferPtr = reinterpret_cast<PixelType*>(buffer);

    // Horizontal pass.
    internalBoxBlur1D(pixels, width, height, range, iterations);
    transpose(pixelsPtr, bufferPtr, width, height);

    // Vertical pass.
    internalBoxBlur1D(buffer, height, width, range, iterations);
    transpose(bufferPtr, pixelsPtr, height, width);

    delete[] buffer;
}
} // namespace image.
NS_EE_END