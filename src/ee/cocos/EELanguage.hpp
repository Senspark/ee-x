//
//  EELanguage.hpp
//  ee-library
//
//  Created by Zinge on 7/21/17.
//
//

#ifndef EE_LIBRARY_LANGUAGE_HPP
#define EE_LIBRARY_LANGUAGE_HPP

#include <string>

namespace ee {
namespace language {
class Language {
private:
    using Self = Language;

public:
    static const Self China;
    static const Self English;
    static const Self Frane;
    static const Self German;
    static const Self Indonesia;
    static const Self Italya;
    static const Self Japan;
    static const Self Korean;
    static const Self Portuguese;
    static const Self Russian;
    static const Self Spanish;
    static const Self Thailand;
    static const Self Vietnamese;

    static Self create(const std::string& code);

    const std::string& getCode() const;

    bool operator==(const Self& other) const;

private:
    explicit Language(const std::string& code);

    std::string code_;
};
} // namespace language
} // namespace ee

#endif /* EE_LIBRARY_LANGUAGE_HPP */
