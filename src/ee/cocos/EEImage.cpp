//
//  EEImage.cpp
//  EE Library
//
//  Created by enrevol on 10/24/15.
//
//

#include "EEImage.hpp"

NS_EE_BEGIN
namespace image {
NS_ANONYMOUS_BEGIN
void internalTranspose(SizeType rowBegin, SizeType rowEnd, SizeType colBegin,
                       SizeType colEnd, PixelType* src, PixelType* dst,
                       SizeType srcWidth, SizeType srcHeight) noexcept {
    auto deltaRow = rowEnd - rowBegin;
    auto deltaCol = colEnd - colBegin;
    if (deltaRow <= 16 && deltaCol <= 16) {
        for (auto col = colBegin; col < colEnd; ++col) {
            for (auto row = rowBegin; row < rowEnd; ++row) {
                dst[col * srcHeight + row] = src[row * srcWidth + col];
            }
        }
    } else if (deltaRow >= deltaCol) {
        internalTranspose(rowBegin, rowBegin + (deltaRow / 2), colBegin, colEnd,
                          src, dst, srcWidth, srcHeight);

        internalTranspose(rowBegin + (deltaRow / 2), rowEnd, colBegin, colEnd,
                          src, dst, srcWidth, srcHeight);
    } else {
        internalTranspose(rowBegin, rowEnd, colBegin, colBegin + (deltaCol / 2),
                          src, dst, srcWidth, srcHeight);

        internalTranspose(rowBegin, rowEnd, colBegin + (deltaCol / 2), colEnd,
                          src, dst, srcWidth, srcHeight);
    }
}
NS_ANONYMOUS_END

void transpose(PixelType* src, PixelType* dst, SizeType srcWidth,
               SizeType srcHeight) noexcept {
    internalTranspose(0, srcHeight, 0, srcWidth, src, dst, srcWidth, srcHeight);
}
} // namespace image
NS_EE_END
