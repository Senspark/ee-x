//
//  EEBoxBlur2D.cpp
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
void internalBoxBlur2D(ChannelType* pixels, SizeType width, SizeType height,
                       SizeType range) {
    auto kernelSize = range * 2 + 1;
    assert(kernelSize < width && kernelSize < height);

    constexpr SizeType Bits = 24;
    const auto Numerator = (1 << Bits) / (kernelSize * kernelSize);

    auto inPixel = pixels;
    auto outPixel = pixels;

    auto vertPrefixSumR = new SizeType[width]();
    auto vertPrefixSumG = new SizeType[width]();
    auto vertPrefixSumB = new SizeType[width]();

    auto horiPrefixSumR = new SizeType[width + 1];
    horiPrefixSumR[0] = 0;
    auto horiPrefixSumG = new SizeType[width + 1];
    horiPrefixSumG[0] = 0;
    auto horiPrefixSumB = new SizeType[width + 1];
    horiPrefixSumB[0] = 0;

    auto buffer = new ChannelType[height * width * 4];

    auto newPixel = buffer;

    auto vertSumR = vertPrefixSumR;
    auto vertSumG = vertPrefixSumG;
    auto vertSumB = vertPrefixSumB;

    auto horiSumInR = horiPrefixSumR;
    auto horiSumInG = horiPrefixSumG;
    auto horiSumInB = horiPrefixSumB;

    auto horiSumOutR = horiPrefixSumR;
    auto horiSumOutG = horiPrefixSumG;
    auto horiSumOutB = horiPrefixSumB;

    auto resetVertSum = [&]() noexcept {
        vertSumR = vertPrefixSumR;
        vertSumG = vertPrefixSumG;
        vertSumB = vertPrefixSumB;
    };
    auto resetHoriSum = [&]() noexcept {
        horiSumInR = horiSumOutR = horiPrefixSumR;
        horiSumInG = horiSumOutG = horiPrefixSumG;
        horiSumInB = horiSumOutB = horiPrefixSumB;
    };
    auto updateHoriSum = [&]() noexcept {
        *(horiSumInR + 1) = *horiSumInR + *vertSumR;
        *(horiSumInG + 1) = *horiSumInG + *vertSumG;
        *(horiSumInB + 1) = *horiSumInB + *vertSumB;
    };
    auto addPixel = [&]() noexcept {
        *vertSumR += *inPixel++;
        *vertSumG += *inPixel++;
        *vertSumB += *inPixel++;
        inPixel++;
    };
    auto removePixel = [&]() noexcept {
        *vertSumR -= *outPixel++;
        *vertSumG -= *outPixel++;
        *vertSumB -= *outPixel++;
        outPixel++;
    };
    auto shiftVertSum = [&]() noexcept {
        ++vertSumR;
        ++vertSumG;
        ++vertSumB;
    };
    auto shiftHoriSumIn = [&]() noexcept {
        ++horiSumInR;
        ++horiSumInG;
        ++horiSumInB;
    };
    auto shiftHoriSumOut = [&]() noexcept {
        ++horiSumOutR;
        ++horiSumOutG;
        ++horiSumOutB;
    };
    auto updatePixel = [&](SizeType hits) noexcept {
        *newPixel++ =
            static_cast<ChannelType>((*horiSumInR - *horiSumOutR) / hits);
        *newPixel++ =
            static_cast<ChannelType>((*horiSumInG - *horiSumOutG) / hits);
        *newPixel++ =
            static_cast<ChannelType>((*horiSumInB - *horiSumOutB) / hits);
        *newPixel++ = std::numeric_limits<ChannelType>::max();
    };
    auto updatePixelFullKernel = [&]() noexcept {
        *newPixel++ = static_cast<ChannelType>(
            ((*horiSumInR - *horiSumOutR) * Numerator) >> Bits);
        *newPixel++ = static_cast<ChannelType>(
            ((*horiSumInG - *horiSumOutG) * Numerator) >> Bits);
        *newPixel++ = static_cast<ChannelType>(
            ((*horiSumInB - *horiSumOutB) * Numerator) >> Bits);
        *newPixel++ = std::numeric_limits<ChannelType>::max();
    };
    for (SizeType row = 0; row < range; ++row) {
        resetVertSum();
        for (SizeType col = 0; col < width; ++col) {
            addPixel();
            shiftVertSum();
        }
    }
    for (SizeType row = range; row <= range + range; ++row) {
        resetVertSum();
        resetHoriSum();
        for (SizeType col = 0; col < range; ++col) {
            addPixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
        }
        for (SizeType col = range; col <= range + range; ++col) {
            addPixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
            updatePixel((row + 1) * (col + 1));
        }
        for (SizeType col = range + range + 1; col < width; ++col) {
            addPixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
            shiftHoriSumOut();
            updatePixel((row + 1) * kernelSize);
        }
        for (SizeType col = 0; col < range; ++col) {
            shiftHoriSumOut();
            updatePixel((row + 1) * (kernelSize - col - 1));
        }
    }
    for (SizeType row = range + range + 1; row < height; ++row) {
        resetVertSum();
        resetHoriSum();
        for (SizeType col = 0; col < range; ++col) {
            addPixel();
            removePixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
        }
        for (SizeType col = range; col <= range + range; ++col) {
            addPixel();
            removePixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
            updatePixel(kernelSize * (col + 1));
        }
        for (SizeType col = range + range + 1; col < width; ++col) {
            addPixel();
            removePixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
            shiftHoriSumOut();
            updatePixelFullKernel();
        }
        for (SizeType col = 0; col < range; ++col) {
            shiftHoriSumOut();
            updatePixel(kernelSize * (kernelSize - col - 1));
        }
    }
    for (SizeType row = 0; row < range; ++row) {
        resetVertSum();
        resetHoriSum();
        for (SizeType col = 0; col < range; ++col) {
            removePixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
        }
        for (SizeType col = range; col <= range + range; ++col) {
            removePixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
            updatePixel((kernelSize - row - 1) * (col + 1));
        }
        for (SizeType col = range + range + 1; col < width; ++col) {
            removePixel();
            updateHoriSum();
            shiftVertSum();
            shiftHoriSumIn();
            shiftHoriSumOut();
            updatePixel((kernelSize - row - 1) * kernelSize);
        }
        for (SizeType col = 0; col < range; ++col) {
            shiftHoriSumOut();
            updatePixel((kernelSize - row - 1) * (kernelSize - col - 1));
        }
    }
    std::memcpy(pixels, buffer, width * height * 4);

    delete[] vertPrefixSumR;
    delete[] vertPrefixSumG;
    delete[] vertPrefixSumB;
    delete[] horiPrefixSumR;
    delete[] horiPrefixSumG;
    delete[] horiPrefixSumB;
    delete[] buffer;
}
NS_ANONYMOUS_END

void boxBlur2D(cocos2d::Image* image, SizeType range) {
    auto pixels = image->getData();
    auto width = static_cast<SizeType>(image->getWidth());
    auto height = static_cast<SizeType>(image->getHeight());
    internalBoxBlur2D(pixels, width, height, range);
}
} // namespace image.
NS_EE_END