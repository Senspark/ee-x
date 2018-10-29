//
//  EEMeta.hpp
//  ee-library
//
//  Created by Zinge on 7/19/17.
//
//

#ifndef EE_LIBRARY_META_HPP
#define EE_LIBRARY_META_HPP

namespace ee {
namespace detail {
template <class... Args> using void_t = void;

/// https://stackoverflow.com/questions/39727409/how-to-simplify-complicated-sfinae-syntax-in-pre-c11-c11-14-and-17
template <long N> struct requires_enum {
    enum class type { none, all };
};

// clang-format off
#define EE_REQUIRES(...)                                                       \
    ::ee::detail::requires_enum<__LINE__>::type =                              \
        ::ee::detail::requires_enum<__LINE__>::type::none,                     \
    typename std::enable_if<(__VA_ARGS__), int>::type = 0
// clang-format on
} // namespace detail
} // namespace ee

#endif /* EE_LIBRARY_META_HPP */
