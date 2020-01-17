/*
 Credit here:

 Copyright (c) 2011, Micael Hildenborg
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Micael Hildenborg nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY Micael Hildenborg ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL Micael Hildenborg BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 Contributors:
 Gustav
 Several members in the gamedev.se forum.
 Gregory Petrosyan
 */

#include "ee/cocos/EECrytography.hpp"

NS_EE_BEGIN
NS_DETAIL_BEGIN
NS_ANONYMOUS_BEGIN
/// Rotate an integer value to left.
std::uint32_t rol(const std::uint32_t value,
                  const std::uint32_t steps) noexcept {
    return (value << steps) | (value >> (32 - steps));
}

/// Sets the first 16 integers in the buffer to zero.
/// Used for clearing the W buffer.
void clearWBuffert(std::uint32_t* buffer) {
    for (std::uint_fast32_t pos = 16; ~(--pos);) {
        buffer[pos] = 0;
    }
}

void innerHash(std::uint32_t* result, std::uint32_t* w) {
    auto a = result[0];
    auto b = result[1];
    auto c = result[2];
    auto d = result[3];
    auto e = result[4];

    std::uint_fast32_t round = 0;

#define sha1macro(func, val)                                                   \
    {                                                                          \
        const std::uint_fast32_t t = rol(a, 5) + (func) + e + val + w[round];  \
        e = d;                                                                 \
        d = c;                                                                 \
        c = rol(b, 30);                                                        \
        b = a;                                                                 \
        a = t;                                                                 \
    }

    while (round < 16) {
        sha1macro((b & c) | (~b & d), 0x5a827999)++ round;
    }
    while (round < 20) {
        w[round] = rol(
            (w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]), 1);
        sha1macro((b & c) | (~b & d), 0x5a827999)++ round;
    }
    while (round < 40) {
        w[round] = rol(
            (w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]), 1);
        sha1macro(b ^ c ^ d, 0x6ed9eba1)++ round;
    }
    while (round < 60) {
        w[round] = rol(
            (w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]), 1);
        sha1macro((b & c) | (b & d) | (c & d), 0x8f1bbcdc)++ round;
    }
    while (round < 80) {
        w[round] = rol(
            (w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]), 1);
        sha1macro(b ^ c ^ d, 0xca62c1d6)++ round;
    }

#undef sha1macro

    result[0] += a;
    result[1] += b;
    result[2] += c;
    result[3] += d;
    result[4] += e;
}
NS_ANONYMOUS_END

/// @param src points to any kind of data to be hashed.
/// @param byteLength is the number of bytes to hash from the src pointer.
/// @param hash should points to a buffer of at least 20 bytes of size
/// for storing the SHA-1 result in.
void calc(const void* src, const std::size_t byteLength, unsigned char* hash);

/// @param hash is 20 bytes of SHA-1 hash.
/// This is the same data that is the result from the calc function.
/// @param hexString should point to a buffer of at least 41 bytes of size
/// for storing the hexadecimal representation of the hash.
/// A zero will be written at position 40, so the buffer will
/// be a valid zero ended string.
void convertByteToHexString(const unsigned char* hash, char* hexString);

void calc(const void* src, const std::size_t byteLength, unsigned char* hash) {
    // Init the result array.
    std::uint32_t result[5] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476,
                               0xc3d2e1f0};

    // Cast the void src pointer to be the byte array we can work with.
    auto sarray = static_cast<const unsigned char*>(src);

    // The reusable round buffer
    std::uint32_t w[80];

    // Loop through all complete 64byte blocks.
    std::size_t endCurrentBlock;
    std::size_t currentBlock = 0;

    while (currentBlock + 64 <= byteLength) {
        endCurrentBlock = currentBlock + 64;

        // Init the round buffer with the 64 byte block data.
        for (int roundPos = 0; currentBlock < endCurrentBlock;
             currentBlock += 4) {
            // This line will swap endian on big endian and keep endian on
            // little endian.
            w[roundPos++] =
                static_cast<std::uint32_t>(sarray[currentBlock + 3]) |
                (static_cast<std::uint32_t>(sarray[currentBlock + 2]) << 8) |
                (static_cast<std::uint32_t>(sarray[currentBlock + 1]) << 16) |
                (static_cast<std::uint32_t>(sarray[currentBlock]) << 24);
        }
        innerHash(result, w);
    }

    // Handle the last and not full 64 byte block if existing.
    endCurrentBlock = byteLength - currentBlock;
    clearWBuffert(w);
    std::size_t lastBlockBytes = 0;
    for (; lastBlockBytes < endCurrentBlock; ++lastBlockBytes) {
        w[lastBlockBytes >> 2] |=
            static_cast<std::uint32_t>(sarray[lastBlockBytes + currentBlock])
            << ((3 - (lastBlockBytes & 3)) << 3);
    }
    w[lastBlockBytes >> 2] |=
        static_cast<std::uint32_t>(0x80 << ((3 - (lastBlockBytes & 3)) << 3));
    if (endCurrentBlock >= 56) {
        innerHash(result, w);
        clearWBuffert(w);
    }
    w[15] = static_cast<std::uint32_t>(byteLength << 3);
    innerHash(result, w);

    // Store hash in result pointer, and make sure we get in in the correct
    // order on both endian models.
    for (std::uint_fast32_t hashByte = 20; ~(--hashByte);) {
        hash[hashByte] =
            (result[hashByte >> 2] >> (((3 - hashByte) & 0x3) << 3)) & 0xff;
    }
}

void convertByteToHexString(const unsigned char* hash, char* hexString) {
    constexpr const char* HexDigits = "0123456789abcdef";

    for (int hashByte = 20; --hashByte >= 0;) {
        hexString[hashByte << 1] = HexDigits[(hash[hashByte] >> 4) & 0xf];
        hexString[(hashByte << 1) + 1] = HexDigits[hash[hashByte] & 0xf];
    }
    hexString[40] = 0;
}
NS_DETAIL_END

std::string generateSha1(const std::string& input) {
    unsigned char hash[20];
    detail::calc(input.c_str(), input.size(), hash);
    char hexString[41];
    detail::convertByteToHexString(hash, hexString);
    return hexString;
}
NS_EE_END
