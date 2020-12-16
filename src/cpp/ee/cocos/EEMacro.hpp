//
//  EEMacro.hpp
//  ee-library
//
//  Created by enrevol.
//
//

#ifndef EE_LIBRARY_MACRO_HPP_
#define EE_LIBRARY_MACRO_HPP_

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
#else // defined(_MSC_VER)
#define COUNT_ARGS(...)                                                        \
    COUNT_ARGS_HELPER(0, ##__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#endif // defined(_MSC_VER)

/// Gets the first argument.
#define GET_FIRST_ARG_HELPER(arg, ...) arg
#define GET_FIRST_ARG(...) GET_FIRST_ARG_HELPER(__VA_ARGS__)

/// Gets all arguments except the first argument.
#define GET_EXCEPT_FIRST_ARG_HELPER(ignore, ...) __VA_ARGS__
#define GET_EXCEPT_FIRST_ARG(...) GET_EXCEPT_FIRST_ARG_HELPER(__VA_ARGS__)

/// Gets overloading function name.
#define GET_FUNCTION(name, arity) STRCAT(name, arity)

#endif // EE_LIBRARY_MACRO_HPP_
