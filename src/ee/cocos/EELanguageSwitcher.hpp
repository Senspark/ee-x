//
//  EELanguageSwitcher.hpp
//  ee-library
//
//  Created by Zinge on 7/21/17.
//
//

#ifndef EE_LIBRARY_LANGUAGE_SWITCHER_HPP
#define EE_LIBRARY_LANGUAGE_SWITCHER_HPP

#include <map>
#include <set>
#include <string>
#include <vector>

#include "EECocos2dxFwd.hpp"
#include "EEILanguageSwitcher.hpp"

namespace ee {
namespace language {
class Formatter;

class Switcher : public ISwitcher {
private:
    using Self = Switcher;

public:
    Switcher();
    virtual ~Switcher();

    /// @see Super.
    virtual const Language& getCurrentLanguage() const override;

    /// @see Super.
    virtual void changeLanguage(const Language& language) override;

    /// @see Super.
    virtual const Formatter&
    getFormatter(const Language& language,
                 const std::string& key) const override;

    /// @see Super.
    virtual bool addObserver(const std::string& key,
                             const Observer& observer) override;

    /// @see Super.
    virtual bool removeObserver(const std::string& key) override;

    /// Loads the specified language from the specified map.
    /// @param[in] language The language to load.
    /// @param[in] map The map which contains the language dictionary.
    void loadLanguage(const Language& language, const cocos2d::ValueMap& map);

    std::vector<Language> getSupportedLanguages() const override;

private:
    std::unique_ptr<Language> currentLanguage_;

    struct LanguageComparator {
        bool operator()(const Language& lhs, const Language& rhs) const;
    };

    std::map<Language, std::map<std::string, Formatter>, LanguageComparator>
        dictionaries_;

    bool locked_;
    std::map<std::string, Observer> observers_;
};
} // namespace language
} // namespace ee

#endif /* EE_LIBRARY_LANGUAGE_SWITCHER_HPP */
