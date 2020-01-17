//
//  EEDialog.h
//  roll-eat
//
//  Created by Hoang Le Hai on 9/1/15.
//
//

#ifndef EE_LIBRARY_DIALOG_HPP_
#define EE_LIBRARY_DIALOG_HPP_

#include <functional>
#include <memory>
#include <vector>

#include "EECocos2dxFwd.hpp"

#include <2d/CCAction.h>
#include <base/CCRefPtr.h>
#include <ui/UIWidget.h>

namespace ee {
namespace dialog {
class Dialog;
class DialogManager;
class DialogTree;
} // namespace dialog

using dialog::Dialog;

namespace dialog {
/// Dialog lifetime:
/// Initial state:
/// - Inactive (isActive() returns false).
///
/// When show() is called.
/// - onDialogWillShow is called.
///   - invoke all callbacks in addDialogWillShowCallback() (can be overridden).
/// - onDialogDidShow is called.
///   - invoke all callbacks in addDialogDidShowCallback() (can be overridden).
/// - Active (isActive() returns true).
///
/// When hide() is called.
/// - Inactive (isActive() returns false).
/// - onDialogWillHide is called.
///   - invoke all callbacks in addDialogWillHideCallback() (can be overridden).
/// - onDialogDidHidee is called.
///   - invoke all callbacks in addDialogDidHideCallback() (can be overridden).
class Dialog : public cocos2d::ui::Widget {
private:
    using Self = Dialog;
    using Super = cocos2d::ui::Widget;

public:
    using TransitionCallback = std::function<void(Self* sender)>;
    using TransitionType = cocos2d::RefPtr<cocos2d::FiniteTimeAction>;

    static const int ContainerLocalZOrder;

    /// Passes this to @c show method to indicate that this dialog should
    /// display at the top level.
    static const std::size_t TopLevel;

    /// Attempts to show this dialog to the current scene.
    /// @param[in] level The dialog level to show, should be in range [1, +inf).
    virtual void show(std::size_t level = TopLevel);

    /// Attempts to hide this dialog from the current scene.
    virtual void hide();

    /// Retrieves the dialog level in the current scene.
    std::size_t getDialogLevel() const noexcept;

    /// Retrieves this dialog's container
    virtual cocos2d::Node* getContainer();

    /// Retrieves this dialog's container (const version).
    virtual const cocos2d::Node* getContainer() const;

    /// Sets whether this dialog is active.
    /// @param[in] active Whether this dialog is active.
    virtual void setActive(bool active);

    /// Checks whether the user can interact with this dialog.
    bool isActive() const noexcept;

    /// Sets whether this dialog should ignore outside touch to autohide
    virtual void setIgnoreTouchOutside(bool ignore);
    
    /// Checks whether the dialog is ignoring outside touch or not
    bool isIgnoreTouchOutside() noexcept;
    
    /// Adds a callback that will invoked when the dialog is about to show.
    /// @param[in] callback The callback.
    /// @param[in] priority The priority of the callback.
    /// @return Instance to this for chaining.
    Self* addDialogWillShowCallback(const TransitionCallback& callback,
                                    int priority = 0);

    /// Adds a callback that will invoked when the dialog has shown.
    /// @param[in] callback The callback.
    /// @param[in] priority The priority of the callback.
    /// @return Instance to this for chaining.
    Self* addDialogDidShowCallback(const TransitionCallback& callback,
                                   int priority = 0);

    /// Adds a callback that will invoked when the dialog is about to hide.
    /// @param[in] callback The callback.
    /// @param[in] priority The priority of the callback.
    /// @return Instance to this for chaining.
    Self* addDialogWillHideCallback(const TransitionCallback& callback,
                                    int priority = 0);

    /// Adds a callback that will invoked when the dialog has hidden.
    /// @param[in] callback The callback.
    /// @param[in] priority The priority of the callback.
    /// @return Instance to this for chaining.
    Self* addDialogDidHideCallback(const TransitionCallback& callback,
                                   int priority = 0);

    /// Adds a transition that will be run when the dialog is showing.
    /// @param[in] transition The transition.
    /// @return Instance to this for chaining.
    Self* addShowingTransition(const TransitionType& transition);

    /// Adds a transition that will be run when the dialog is hiding.
    /// @param[in] transition The transition.
    /// @return Instance to this for chaining.
    Self* addHidingTransition(const TransitionType& transition);

protected:
    friend DialogManager;
    friend DialogTree;

    Dialog();
    virtual ~Dialog() override;

    virtual bool init() override;

    virtual void onEnter() override;
    virtual void onExit() override;

    /// Invoked when the user clicked the outside boundary of this dialog.
    virtual void onClickedOutside();

    virtual void onDialogWillShow();
    virtual void onDialogDidShow();
    virtual void onDialogWillHide();
    virtual void onDialogDidHide();

    virtual bool hitTest(const cocos2d::Point& pt,
                         const cocos2d::Camera* camera,
                         cocos2d::Vec3* p) const override;

private:
    using CallbackInfo = std::pair<TransitionCallback, int>;

    void invokeCallbacks(std::vector<CallbackInfo>& callbacks);
    void onPressed();

    std::size_t dialogLevel_;
    bool isActive_;
    bool ignoreTouchOutside_;

    cocos2d::Node* transitionAction_;

    std::vector<TransitionType> showingTransitions_;
    std::vector<TransitionType> hidingTransitions_;

    std::vector<CallbackInfo> dialogWillShowCallbacks_;
    std::vector<CallbackInfo> dialogDidShowCallbacks_;
    std::vector<CallbackInfo> dialogWillHideCallbacks_;
    std::vector<CallbackInfo> dialogDidHideCallbacks_;
};
} // namespace dialog.
} // namespace ee

#endif /* EE_LIBRARY_DIALOG_HPP_ */
