//
//  EEMacro.hpp
//  ee-library
//
//  Created by enrevol.
//
//

#ifndef EE_LIBRARY_MACRO_HPP_
#define EE_LIBRARY_MACRO_HPP_

#ifndef NS_EE
#define NS_EE ee
#endif

#define NS_EE_BEGIN namespace NS_EE {
#define NS_EE_END }

#define NS_DETAIL_BEGIN namespace detail {
#define NS_DETAIL_END }

#define NS_ANONYMOUS_BEGIN namespace {
#define NS_ANONYMOUS_END }

// cocosbuilder member variable assigner glue weak.
#define CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK_EX(variableName)                  \
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(                                      \
        this, #variableName,                                                   \
        std::remove_reference_t<decltype(_##variableName)>, _##variableName)

#define CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK_EX_2(variableName)                \
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(                                      \
        this,                                                                  \
        [](const std::string& input) {                                         \
        auto from = input.find_first_not_of('_');                              \
        auto to = input.find_last_not_of('_');                                 \
        return input.substr(from, to - from + 1);                              \
        }(#variableName).c_str(),                                              \
        std::remove_reference_t<decltype(variableName)>, variableName)

#define CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK_EX_3(variableName, variable)      \
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(                                      \
        this, variableName, std::remove_reference_t<decltype(variable)>,       \
        variable)

/// Concatenates two string literals.
#define STRCAT_HELPER(x, y) x##y
#define STRCAT(x, y) STRCAT_HELPER(x, y)

/// Counts number of arguments.
#define COUNT_ARGS_HELPER(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N
#if defined(_MSC_VER)
#define COUNT_ARGS_MSC(...) COUNT_ARGS_HELPER __VA_ARGS__
#define COUNT_ARGS(...)                                                        \
    COUNT_ARGS_MSC((0, ##__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#else
#define COUNT_ARGS(...)                                                        \
    COUNT_ARGS_HELPER(0, ##__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#endif

/// Gets the first argument.
#define GET_FIRST_ARG_HELPER(arg, ...) arg
#define GET_FIRST_ARG(...) GET_FIRST_ARG_HELPER(__VA_ARGS__)

/// Gets all arguments except the first argument.
#define GET_EXCEPT_FIRST_ARG_HELPER(ignore, ...) __VA_ARGS__
#define GET_EXCEPT_FIRST_ARG(...) GET_EXCEPT_FIRST_ARG_HELPER(__VA_ARGS__)

/// Gets overloading function name.
#define GET_FUNCTION(name, arity) STRCAT(name, arity)

#if defined(_MSC_VER) || defined(__clang__) || defined(__GNUC__)
#if defined(_MSC_VER)
#define FUNCTION_SIGNATURE __FUNCSIG__
#elif defined(__clang__) || defined(__GNUC__)
#define FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#endif

/// Simple function logging.
#define LOG_FUNC() CCLOG("%s.", FUNCTION_SIGNATURE)

/// Simple function logging with format.
#define LOG_FUNC_FORMAT(format, ...)                                           \
    CCLOG("%s: " format ".", FUNCTION_SIGNATURE, ##__VA_ARGS__)

/// Full function logging.
#define LOG_FULL()                                                             \
    CCLOG("file = %s\nline = %d\nfunc = %s.", __FILE__, __LINE__,              \
          FUNCTION_SIGNATURE)

/// Full function loggin with format.
#define LOG_FULL_FORMAT(format, ...)                                           \
    CCLOG("file = %s\nline = %d\nfunc = %s\n" format ".", __FILE__, __LINE__,  \
          FUNCTION_SIGNATURE, ##__VA_ARGS__)

#else
#define LOG_FUNC()
#define LOG_FUNC_FORMAT(format, ...)
#define LOG_FULL()
#define LOG_FULL_FORMAT(format, ...)
#endif

#endif // EE_LIBRARY_MACRO_HPP_
