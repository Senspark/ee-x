//
//  EEDialogMessage.cpp
//  ee-library
//
//  Created by Zinge on 5/11/16.
//
//

#include "ee/cocos/DialogCommand.hpp"

#include "ee/cocos/Dialog.hpp"

namespace ee {
namespace cocos {
DialogCommand::DialogCommand(DialogCommandType type_, Dialog* dialog_,
                             std::size_t level_)
    : type(type_)
    , dialog(dialog_)
    , level(level_)
    , guard_(dialog) {}
} // namespace cocos
} // namespace ee
