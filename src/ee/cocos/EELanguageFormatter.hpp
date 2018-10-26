//
//  LanguageFormatter.hpp
//  ee-library
//
//  Created by Zinge on 7/27/17.
//
//

#ifndef EE_LIBRARY_LANGUAGE_FORMATTER_HPP
#define EE_LIBRARY_LANGUAGE_FORMATTER_HPP

#include <string>
#include <vector>

namespace ee {
namespace language {
class Formatter {
private:
    using Self = Formatter;

public:
    /// Constructs a language formatter.
    /// @param[in] format The input format string.
    explicit Formatter(const std::string& format);

    const std::string& getFormat() const { return raw_; }

    std::size_t getPlaceholders() const { return placeholders_; }

    /// Prints the format string.
    std::string format() const;

    /// Prints the format string.
    /// @param[in] args The format arguments.
    std::string format(const std::vector<std::string>& args) const;

protected:
    /// Sets the specified format string.
    /// @param[in] format The input format string.
    void setFormat(const std::string& format);

private:
    /// Raw format string.
    std::string raw_;

    std::size_t placeholders_;
    std::vector<std::string> tokens_;
};
} // namespace language
} // namespace ee

#endif /* EE_LIBRARY_LANGUAGE_FORMATTER_HPP */
