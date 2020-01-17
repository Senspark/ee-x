//
//  ILanguageSwitcher.hpp
//  ee-library
//
//  Created by eps on 6/1/18.
//

#ifndef EE_LIBRARY_I_LANGUAGE_SWITCHER_HPP
#define EE_LIBRARY_I_LANGUAGE_SWITCHER_HPP

#include <functional>
#include <string>

namespace ee {
namespace language {
class Language;
class Formatter;

class ISwitcher {
public:
    using Observer = std::function<void(const Language& language)>;

    ISwitcher() = default;
    virtual ~ISwitcher() = default;

    /// Gets the current language.
    virtual const Language& getCurrentLanguage() const = 0;

    /// Changes the current language to the specified language.
    /// @param[in] language The desired language.
    virtual void changeLanguage(const Language& language) = 0;

    /// Gets the formatter for the specified language and key.
    /// @param[in] language The desired language.
    /// @param[in] key The multilingual key.
    /// @return The formatter.
    virtual const Formatter& getFormatter(const Language& language,
                                          const std::string& key) const = 0;

    /// Adds an obserser which observes when the current language has changed.
    /// @param[in] key The observer's key.
    virtual bool addObserver(const std::string& key,
                             const Observer& observer) = 0;

    /// Removes an observer whose the specified key.
    /// @param[in] key The observer's key.
    virtual bool removeObserver(const std::string& key) = 0;

    /// Get supported languages
    virtual std::vector<Language> getSupportedLanguages() const = 0;
};
} // namespace language
} // namespace ee

#endif /* EE_LIBRARY_E_LANGUAGE_SWITCHER_HPP */
