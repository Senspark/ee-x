//
//  EELanguageSwitcher.cpp
//  ee-library
//
//  Created by Zinge on 7/21/17.
//
//

#include "EELanguageSwitcher.hpp"

#include <base/CCValue.h>

#include "EELanguage.hpp"
#include "EELanguageDelegate.hpp"
#include "EELanguageFormatter.hpp"

namespace ee {
namespace language {
using Self = Switcher;

bool Self::LanguageComparator::operator()(const Language& lhs,
                                          const Language& rhs) const {
    return lhs.getCode() < rhs.getCode();
}

Self::Switcher() {
    locked_ = false;
    currentLanguage_ = std::make_unique<Language>(Language::English);
}

Self::~Switcher() {}

const Language& Self::getCurrentLanguage() const {
    return *currentLanguage_;
}

void Self::changeLanguage(const Language& language) {
    CC_ASSERT(not locked_);
    locked_ = true;
    currentLanguage_ = std::make_unique<Language>(language);
    for (auto&& [key, observer] : observers_) {
        observer(language);
    }
    locked_ = false;
}

const Formatter& Self::getFormatter(const Language& language,
                                    const std::string& key) const {
    try {
        auto&& result = dictionaries_.at(language).at(key);
        return result;
    } catch (const std::out_of_range& ex) {
        CC_ASSERT(false);
        static const Formatter nil("{null}");
        return nil;
    }
}

bool Self::addObserver(const std::string& key, const Observer& observer) {
    CC_ASSERT(not locked_);
    if (observers_.count(key) != 0) {
        return false;
    }
    observers_.emplace(key, observer);
    return true;
}

bool Self::removeObserver(const std::string& key) {
    CC_ASSERT(not locked_);
    if (observers_.count(key) == 0) {
        return false;
    }
    observers_.erase(key);
    return true;
}

void Self::loadLanguage(const Language& language,
                        const cocos2d::ValueMap& map) {
    for (auto&& elt : map) {
        auto&& key = elt.first;
        auto&& text = elt.second.asString();
        auto formatter = Formatter(text);
        dictionaries_[language].emplace(key, std::move(formatter));
    }
}

std::vector<Language> Self::getSupportedLanguages() const {
    return std::vector<Language>();
}
} // namespace language
} // namespace ee
