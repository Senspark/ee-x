//
//  EEUiWidgetCallback.hpp
//  ee-library
//
//  Created by enrevol on 3/23/16.
//
//

#ifndef EE_LIBRARY_UI_WIDGET_CALLBACK_HPP_
#define EE_LIBRARY_UI_WIDGET_CALLBACK_HPP_

#include "EEMacro.hpp"

#include <GUI/CCControlExtension/CCControl.h>
#include <ui/UIWidget.h>

NS_EE_BEGIN
NS_DETAIL_BEGIN
struct DummyRef : public cocos2d::Ref {
    void dummyFunction(cocos2d::Ref*) {}
    void dummyFunction(cocos2d::Ref*, cocos2d::extension::Control::EventType) {}
};

/// Converts touch callback
/// from @c cocos2d::extension::ControlButton
/// to @c cocos2d::ui::Button.
class UiWidgetCallback {
private:
    using Self = UiWidgetCallback;

public:
    using TouchCallback = cocos2d::ui::Widget::ccWidgetTouchCallback;
    using ClickCallback = cocos2d::ui::Widget::ccWidgetClickCallback;

    static Self* getInstance();

    void setActiveTouchCallback(const TouchCallback& callback);
    void setActiveClickCallback(const ClickCallback& callback);

    const TouchCallback& getActiveTouchCallback() const;
    const ClickCallback& getActiveClickCallback() const;

private:
    TouchCallback touchCallback_;
    ClickCallback clickCallback_;
};

#define CCB_SELECTORRESOLVER_CCBUTTON_TOUCH(target, selectorName, callback)    \
    if (pTarget == target && std::strcmp(pSelectorName, selectorName) == 0) {  \
        ee::detail::UiWidgetCallback::getInstance()->setActiveTouchCallback(   \
            callback);                                                         \
        return CC_MENU_SELECTOR(ee::detail::DummyRef::dummyFunction);          \
    }

#define CCB_SELECTORRESOLVER_CCBUTTON_TOUCH_EX(selectorName, callback)         \
    CCB_SELECTORRESOLVER_CCBUTTON_TOUCH(this, selectorName, callback)

#define CCB_SELECTORRESOLVER_CCBUTTON_CLICK(target, selectorName, callback)    \
    if (pTarget == target && std::strcmp(pSelectorName, selectorName) == 0) {  \
        ee::detail::UiWidgetCallback::getInstance()->setActiveClickCallback(   \
            callback);                                                         \
        return CC_MENU_SELECTOR(ee::detail::DummyRef::dummyFunction);          \
    }

#define CCB_SELECTORRESOLVER_CCBUTTON_CLICK_EX_0(function)                     \
    {                                                                          \
        std::string str = #function;                                           \
        str = str.substr(str.rfind("::") + 2);                                 \
        CCB_SELECTORRESOLVER_CCBUTTON_CLICK(this, str.c_str(),                 \
                                            CC_CALLBACK_0(function, this));    \
    }

#define CCB_SELECTORRESOLVER_CCBUTTON_CLICK_EX_1(function)                     \
    {                                                                          \
        std::string str = #function;                                           \
        str = str.substr(str.rfind("::") + 2);                                 \
        CCB_SELECTORRESOLVER_CCBUTTON_CLICK(this, str.c_str(),                 \
                                            CC_CALLBACK_1(function, this));    \
    }
NS_DETAIL_END
NS_EE_END

#endif /* EE_LIBRARY_UI_WIDGET_CALLBACK_HPP_ */
