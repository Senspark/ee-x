//
//  EEDialogTree.hpp
//  ee-library
//
//  Created by Zinge on 7/27/17.
//
//

#ifndef EE_X_DIALOG_MANAGER_HPP
#define EE_X_DIALOG_MANAGER_HPP

#include <cstddef>
#include <vector>

#include "ee/cocos/IDialogManager.hpp"

namespace ee {
namespace cocos {
class DialogManager : public IDialogManager {
private:
    using Self = DialogManager;

public:
    explicit DialogManager(cocos2d::Node* root);
    virtual ~DialogManager() override;

    virtual void pushDialog(Dialog* dialog) override;
    virtual void pushDialog(Dialog* dialog, std::size_t level) override;

    virtual void popDialog(Dialog* dialog) override;
    virtual void popDialog() override;

    virtual Dialog* getDialog(std::size_t level) const override;
    virtual Dialog* getTopDialog() const override;

    virtual std::size_t getLevel() const override;

private:
    /// Resets the dialog tree.
    void reset();

    cocos2d::Node* getRunningNode() const;

    /// Checks whether there is a locking dialog.
    bool isLocked() const;

    /// Locks the command queue with the specified dialog.
    /// @param[in] dialog The dialog to be locked.
    void lock(const Dialog* dialog);

    /// Unlocks the command queue with the specified dialog.
    /// @param[in] dialog The dialog to be unlocked.
    void unlock(const Dialog* dialog);

    /// Processes available dialog command queue.
    /// @return Whether any command was executed.
    bool processCommandQueue();

    /// Processes the specified command.
    /// @param command The command to be processed.
    /// @return Whether the command was executed.
    bool processCommand(const DialogCommand& command);

    /// Processes a push command.
    /// @param[in] dialog The dialog to be pushed.
    /// @param[in] level The level to be considered.
    /// @return Whether the command was executed.
    bool processPushCommand(Dialog* dialog, std::size_t level);

    /// Processes a pop command.
    /// @param[in] dialog The dialog to be popped.
    /// @param[in] level The level to be considered.
    /// @return Whether the command was executed.
    bool processPopCommand(Dialog* dialog, std::size_t level);

    /// Pushes a command to the command queue.
    /// @param[in] command The command to be pushed.
    /// @return Whether the command was pushed.
    bool pushCommand(const DialogCommand& command);

    void pushDialogImmediately(Dialog* dialog, std::size_t level);

    void popDialogImmediately(Dialog* dialog);

    std::size_t currentLevel_;

    cocos2d::Node* root_;

    const Dialog* lockingDialog_;

    std::vector<DialogCommand> commandQueue_;

    std::vector<DialogGuard> dialogStack_;
};
} // namespace cocos
} // namespace ee

#endif /* EE_X_DIALOG_MANAGER_HPP */
