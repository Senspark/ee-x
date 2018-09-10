//
//  EECrytography.hpp
//  ee-library
//
//  Created by enrevol on 10/27/15.
//
//

#ifndef EE_LIBRARY_CRYTOGRAPHY_HPP_
#define EE_LIBRARY_CRYTOGRAPHY_HPP_

#include <string>

#include "EEMacro.hpp"

NS_EE_BEGIN
/// Generates SHA-1 hashed string.
/// @param input is the string to be hashed.
/// @return SHA-1 hashed hexadecimal string.
std::string generateSha1(const std::string& input);

/// Generates CRC hashed string.
/// @param input is the string to be hashed.
/// @return 32bit CRC hashed hexadecimal string.
std::string generateCrc(const std::string& input);

std::string generateMd5(const std::string& input);
NS_EE_END

#endif /* EE_LIBRARY_CRYTOGRAPHY_HPP_ */
