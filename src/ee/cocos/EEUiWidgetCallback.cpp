//
//  EEUiWidgetCallback.cpp
//  ee-library
//
//  Created by enrevol on 3/23/16.
//
//

#include "EEUiWidgetCallback.hpp"

NS_EE_BEGIN
NS_DETAIL_BEGIN
UiWidgetCallback* UiWidgetCallback::getInstance() {
    static Self sharedInstance;
    return &sharedInstance;
}

void UiWidgetCallback::setActiveClickCallback(const ClickCallback& callback) {
    clickCallback_ = callback;
    touchCallback_ = nullptr;
}

void UiWidgetCallback::setActiveTouchCallback(const TouchCallback& callback) {
    touchCallback_ = callback;
    clickCallback_ = nullptr;
}

auto UiWidgetCallback::getActiveTouchCallback() const -> const TouchCallback & {
    return touchCallback_;
}

auto UiWidgetCallback::getActiveClickCallback() const -> const ClickCallback & {
    return clickCallback_;
}
NS_DETAIL_END
NS_EE_END
