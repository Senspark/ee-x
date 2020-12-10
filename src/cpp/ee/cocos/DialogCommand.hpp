//
//  EEDialogMessage.hpp
//  ee-library
//
//  Created by Zinge on 5/11/16.
//
//

#ifndef EE_X_DIALOG_COMMAND_HPP
#define EE_X_DIALOG_COMMAND_HPP

#ifdef __cplusplus

#include <cstddef>

#include "ee/cocos/DialogGuard.hpp"

namespace ee {
namespace cocos {
enum class DialogCommandType {
    Push,
    Pop,
};

/// Stores a dialog command.
///
/// The dialog and its container will be retained.
struct DialogCommand final {
public:
    explicit DialogCommand(DialogCommandType type, Dialog* dialog,
                           std::size_t level);

    DialogCommandType type;
    Dialog* dialog;
    std::size_t level;

private:
    DialogGuard guard_;
};
} // namespace cocos
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_DIALOG_COMMAND_HPP */
