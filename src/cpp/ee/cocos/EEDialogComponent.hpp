//
//  EEDialogComponent.hpp
//  ee-library
//
//  Created by Zinge on 3/23/17.
//
//

#ifndef EE_LIBRARY_DIALOG_COMPONENT_HPP_
#define EE_LIBRARY_DIALOG_COMPONENT_HPP_

#ifdef __cplusplus

#include <2d/CCComponent.h>

#include "ee/cocos/CocosFwd.hpp"

namespace ee {
namespace cocos {
/// DialogManager will pause and resume children nodes when any dialog is pushed
/// or popped.
///
/// Use this class to override that behaviour.
/// Usage:
/// @code
/// your_node->addComponent(
///     ee::DialogComponent::create()
///         ->setResumeCallback([your_node](ee::Dialog* poppedDialog) {
///             // poppedDialog is the dialog that is hiding.
///             // Default behaviour: your_node->resume();
///             // Override your behaviour here.
///         })
///         ->setPauseCallback([your_node](ee::Dialog* pushedDialog) {
///             // pushedDialog is the dialog that is showing.
///             // Default behaviour: your_node->pause();
///             // Override your behaviour here.
///         }));
/// @endcode
class DialogComponent : public cocos2d::Component {
private:
    using Self = DialogComponent;
    using Super = cocos2d::Component;

public:
    using ResumeCallback = std::function<void(Dialog* dialog)>;
    using PauseCallback = std::function<void(Dialog* dialog)>;

    static const std::string DefaultName;

    static Self* create();

    void resume(Dialog* dialog);
    void pause(Dialog* dialog);

    Self* setResumeCallback(const ResumeCallback& callback);
    Self* setPauseCallback(const PauseCallback& callback);

private:
    virtual bool init() override;

    ResumeCallback resumeCallback_;
    PauseCallback pauseCallback_;
};
} // namespace cocos
} // namespace ee

#endif // __cplusplus

#endif /* EE_LIBRARY_DIALOG_COMPONENT_HPP_ */
