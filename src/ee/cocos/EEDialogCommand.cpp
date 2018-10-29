//
//  EEDialogMessage.cpp
//  ee-library
//
//  Created by Zinge on 5/11/16.
//
//

#include "EEDialogCommand.hpp"
#include "EEDialog.hpp"

namespace ee {
namespace dialog {
Command::Command(CommandType type_, Dialog* dialog_, std::size_t level_)
    : type(type_)
    , dialog(dialog_)
    , level(level_)
    , guard_(dialog) {
}
} // namespace dialog
} // namespace ee
