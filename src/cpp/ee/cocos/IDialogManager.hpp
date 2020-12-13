#ifndef EE_X_I_DIALOG_MANAGER_HPP
#define EE_X_I_DIALOG_MANAGER_HPP

#ifdef __cplusplus

#include "ee/cocos/CocosFwd.hpp"

namespace ee {
namespace cocos {
class IDialogManager {
public:
    virtual ~IDialogManager() = default;

    /// Pushes the specified dialog to the top level.
    /// @param[in] dialog The dialog to be pushed.
    virtual void pushDialog(Dialog* dialog) = 0;

    /// Pushes the specified dialog to the specified level.
    /// @param[in] dialog The dialog to be pushed.
    /// @param[in] level The dialog level.
    virtual void pushDialog(Dialog* dialog, std::size_t level) = 0;

    /// Pops the specified dialog.
    /// @param[in] dialog The dialog to be popped.
    virtual void popDialog(Dialog* dialog) = 0;

    /// Pops the top dialog.
    virtual void popDialog() = 0;

    /// Gets the dialog whose the specified level.
    /// @param[in] level The desired level.
    /// @return The dialog whose the specified level if exists, nullptr
    /// otherwise.
    virtual Dialog* getDialog(std::size_t level) const = 0;

    /// Gets the top dialog.
    virtual Dialog* getTopDialog() const = 0;

    /// Gets the dialog level.
    virtual std::size_t getLevel() const = 0;
};
} // namespace cocos
} // namespace ee

#endif // __cplusplus

#endif // EE_X_I_DIALOG_MANAGER_HPP
