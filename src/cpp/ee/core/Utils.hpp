//
//  Utils.hpp
//  ee_x
//
//  Created by Zinge on 10/9/17.
//
//

#ifndef EE_X_UTILS_HPP
#define EE_X_UTILS_HPP

#ifdef __cplusplus

#include <cstdarg>
#include <functional>
#include <string>

#include "ee/core/CoreFwd.hpp"

namespace ee {
namespace core {
// lower string
[[deprecated]] std::string str_tolower(std::string s);

/// Converts float to string without trailing zeroes.
std::string toString(float value);

/// Converts bool to string, used internally.
std::string toString(bool value);

/// Convert string to bool, used internally.
bool toBool(const std::string& value);

/// http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
std::string format(std::string formatString, ...);
std::string format(std::string formatString, std::va_list args);

/// bit_cast.
///
/// https://gist.github.com/socantre/3472964
template <class Dest, class Source>
inline Dest bitCast(const Source& source) {
    static_assert(sizeof(Dest) == sizeof(Source),
                  "size of destination and source objects must be equal");

    static_assert(std::is_trivially_copyable<Dest>::value,
                  "destination type must be trivially copyable");

    static_assert(std::is_trivially_copyable<Source>::value,
                  "source type must be trivially copyable");
    Dest dest;
    std::memcpy(&dest, &source, sizeof(dest));
    return dest;
}

/// Logs a message using __android_log_print (Android) and NSLog (iOS).
void log(const LogLevel& level, const std::string& tag,
         const std::string& message);

std::string dumpBacktrace(size_t count);
} // namespace core

using core::bitCast;
using core::format;
using core::log;
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_UTILS_HPP */
