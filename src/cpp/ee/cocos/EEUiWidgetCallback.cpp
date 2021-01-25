//
//  EEUiWidgetCallback.cpp
//  ee-library
//
//  Created by enrevol on 3/23/16.
//
//

#include "ee/cocos/EEUiWidgetCallback.hpp"

#ifdef EE_X_COCOS_CPP

namespace ee {
namespace detail {
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

auto UiWidgetCallback::getActiveTouchCallback() const -> const TouchCallback& {
    return touchCallback_;
}

auto UiWidgetCallback::getActiveClickCallback() const -> const ClickCallback& {
    return clickCallback_;
}
} // namespace detail
} // namespace ee

#endif // EE_X_COCOS_CPP