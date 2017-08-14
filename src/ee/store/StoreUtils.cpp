//
//  StoreUtils.cpp
//  ee_x
//
//  Created by Zinge on 5/19/17.
//
//

#include <cstdarg>
#include <cstdlib>

#include "ee/store/StoreUtils.hpp"

namespace ee {
namespace store {
std::string format(std::string fmt_str, ...) {
    int final_n;
    auto n = 2 * fmt_str.size(); /* Reserve two times as much as the length of
                                    the fmt_str */
    std::string str;
    std::unique_ptr<char[]> formatted;
    std::va_list ap;
    while (1) {
        formatted.reset(
            new char[n]); /* Wrap the plain char array into the unique_ptr */
        std::strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        final_n = std::vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= static_cast<int>(n))
            n += static_cast<std::size_t>(
                std::abs(final_n - static_cast<int>(n) + 1));
        else
            break;
    }
    return std::string(formatted.get());
}
} // namespace store
} // namespace ee
