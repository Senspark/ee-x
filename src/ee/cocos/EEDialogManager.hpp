//
//  EEDialogManager.h
//  roll-eat
//
//  Created by Hoang Le Hai on 8/27/15.
//
//

#ifndef EE_LIBRARY_DIALOG_MANAGER_HPP_
#define EE_LIBRARY_DIALOG_MANAGER_HPP_

#include <memory>
#include <vector>
#include <map>

#include "EECocos2dxFwd.hpp"

#include <base/CCRefPtr.h>

namespace ee {
namespace dialog {
enum class CommandType;

class Dialog;
class Command;
class Guard;
class DialogManager;
class DialogTree;
} // namespace dialog

using dialog::DialogManager;

namespace dialog {
/// Dialog manager manages the dialogs in the scene.
class DialogManager {
private:
    using Self = DialogManager;

public:
    static Self* getInstance();

    /// @see DialogTree::pushDialog.
    void pushDialog(Dialog* dialog);

    /// @see DialogTree::pushDialog.
    void pushDialog(Dialog* dialog, std::size_t level);

    /// @see DialogTree::popDialog.
    void popDialog(Dialog* dialog);

    /// @see DialogTree::popDialog.
    void popDialog();

    /// @see DialogTree::getDialog.
    Dialog* getDialog(std::size_t level);

    /// @see DialogTree::getTopDialog.
    Dialog* getTopDialog();

    std::size_t getTopDialogLevel();

private:
    DialogManager();
    ~DialogManager();

    void updateTree();

    std::unique_ptr<DialogTree>& getTree(cocos2d::Scene* scene);
    std::unique_ptr<DialogTree>& getCurrentTree();

    std::map<cocos2d::RefPtr<cocos2d::Scene>, std::unique_ptr<DialogTree>>
        trees_;
};
} // namespace dialog.
} // namespace ee

#endif /* EE_LIBRARY_DIALOG_MANAGER_HPP_ */
