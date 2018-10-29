//
//  EELanguageDelegate.cpp
//  ee-library
//
//  Created by Zinge on 7/21/17.
//
//

#include "EELanguageDelegate.hpp"

#include "EEILanguageSwitcher.hpp"
#include "EELanguage.hpp"
#include "EELanguageFormatter.hpp"

namespace ee {
namespace language {
using Self = Delegate;

Self::Delegate()
    : switcher_(nullptr) {
    static int id = 0; // Unique ID for each language delegate.
    id_ = std::to_string(id++);
}

Self::~Delegate() {
    if (switcher_ != nullptr) {
        switcher_->removeObserver(id_);
    }
}

const Language& Self::getLanguage() const {
    if (language_) {
        return *language_;
    }
    static auto nil = Language::create("_nil_");
    return nil;
}

Self* Self::setSwitcher(ISwitcher& switcher) {
    if (switcher_ != nullptr) {
        // Remove observer from old switcher.
        switcher_->removeObserver(id_);
    }
    switcher_ = std::addressof(switcher);
    setLanguage(switcher_->getCurrentLanguage());
    // Add observer to the new switcher.
    switcher_->addObserver(id_,
                           [this](const Language& language) { //
                               setLanguage(language);
                           });
    return this;
}

Self* Self::setKey(const std::string& key) {
    if (key_ && *key_ != key) {
        args_.reset();
    }
    key_ = std::make_unique<std::string>(key);
    auto&& formatter = switcher_->getFormatter(getLanguage(), key);
    if (formatter.getPlaceholders() == 0) {
        // Empty format.
        setFormat({});
    } else {
        updateText();
    }
    return this;
}

Self* Self::setFormat(const std::vector<std::string>& args) {
    args_ = std::make_unique<std::vector<std::string>>(args);
    updateText();
    return this;
}

Self* Self::setLanguage(const Language& language) {
    language_ = std::make_unique<Language>(language);
    updateText();
    return this;
}

Self* Self::setTextCallback(const TextCallback& callback) {
    textCallback_ = callback;
    updateText();
    return this;
}

void Self::updateText() {
    if (not textCallback_) {
        return;
    }
    if (not language_) {
        return;
    }
    if (not key_) {
        return;
    }
    if (not args_) {
        return;
    }
    auto&& formatter = switcher_->getFormatter(getLanguage(), *key_);
    auto&& text = formatter.format(*args_);
    textCallback_(text);
}
} // namespace language
} // namespace ee
