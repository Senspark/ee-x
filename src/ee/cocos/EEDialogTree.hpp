//
//  EEDialogTree.hpp
//  ee-library
//
//  Created by Zinge on 7/27/17.
//
//

#ifndef EE_LIBRARY_DIALOG_TREE_HPP
#define EE_LIBRARY_DIALOG_TREE_HPP

#include <cstddef>
#include <vector>

#include "EECocos2dxFwd.hpp"

namespace ee {
namespace dialog {
class Dialog;
class Command;
class Guard;

class DialogTree {
private:
    using Self = DialogTree;

public:
    explicit DialogTree(cocos2d::Scene* scene);
    ~DialogTree();

    /// Pushes the specified dialog to the top level.
    /// @param[in] dialog The dialog to be pushed.
    void pushDialog(Dialog* dialog);

    /// Pushes the specified dialog to the specified level.
    /// @param[in] dialog The dialog to be pushed.
    /// @param[in] level The dialog level.
    void pushDialog(Dialog* dialog, std::size_t level);

    /// Pops the specified dialog.
    /// @param[in] dialog The dialog to be popped.
    void popDialog(Dialog* dialog);

    /// Pops the top dialog.
    void popDialog();

    /// Gets the dialog whose the specified level.
    /// @param[in] level The desired level.
    /// @return The dialog whose the specified level if exists, nullptr
    /// otherwise.
    Dialog* getDialog(std::size_t level);

    /// Gets the top dialog.
    Dialog* getTopDialog();

    /// Gets the dialog level.
    std::size_t getLevel() const;

private:
    /// Resets the dialog tree.
    void reset();

    cocos2d::Node* getRunningNode();

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
    bool processCommand(const Command& command);

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
    bool pushCommand(const Command& command);

    void pushDialogImmediately(Dialog* dialog, std::size_t level);

    void popDialogImmediately(Dialog* dialog);

    std::size_t currentLevel_;

    cocos2d::Scene* scene_;

    const Dialog* lockingDialog_;

    std::vector<Command> commandQueue_;

    std::vector<Guard> dialogStack_;
};
} // namespace
} // namespace ee

#endif /* EE_LIBRARY_DIALOG_TREE_HPP */
