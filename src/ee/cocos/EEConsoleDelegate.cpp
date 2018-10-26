//
//  EEConsoleDelegate.cpp
//  ee-library
//
//  Created by Zinge on 4/10/17.
//
//

#include <sstream>

#include "EEConsoleDelegate.hpp"
#include "EEConsole.hpp"
#include "EECommandPool.hpp"

namespace ee {
namespace detail {
ConsoleDelegate::ConsoleDelegate(Console* console)
    : console_(console) {
}

void ConsoleDelegate::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) {
    //
}

void ConsoleDelegate::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox) {
    //
}

void ConsoleDelegate::editBoxTextChanged(cocos2d::ui::EditBox* editBox,
                                         const std::string& text) {
    //
}

void ConsoleDelegate::editBoxReturn(cocos2d::ui::EditBox* editBox) {
    // Parse tokens.
    std::stringstream ss(editBox->getText());
    std::vector<std::string> tokens;
    for (std::string token; ss >> token;) {
        std::transform(token.cbegin(), token.cend(), token.begin(), ::toupper);
        tokens.push_back(token);
    }

    for (auto&& pool : console_->pools_) {
        if (pool->process(tokens)) {
            break;
        }
    }

    editBox->setText("");
}
} // namespace detail
} // namespace ee
