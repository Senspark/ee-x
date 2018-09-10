//
//  EEWidgetLoader.cpp
//  ee-library
//
//  Created by eps on 6/4/18.
//

#include "EEWidgetLoader.hpp"

#include "EEWidget.hpp"

namespace ee {
namespace ui {
using Self = WidgetLoader;

namespace property {
constexpr auto inset_left = "insetLeft";
constexpr auto inset_top = "insetTop";
constexpr auto inset_right = "insetRight";
constexpr auto inset_bottom = "insetBottom";
} // namespace property

cocos2d::Node* Self::createNode(cocos2d::Node* parent,
                                cocosbuilder::CCBReader* reader) {
    auto result = Widget::create();
    return result;
}

void Self::onHandlePropTypeFloat(cocos2d::Node* node_, cocos2d::Node* parent,
                                 const char* name_, float value,
                                 cocosbuilder::CCBReader* reader) {
    auto node = dynamic_cast<Widget*>(node_);
    std::string name(name_);
    if (name == property::inset_left) {
        return node->setInsetLeft(value);
    }
    if (name == property::inset_top) {
        return node->setInsetTop(value);
    }
    if (name == property::inset_right) {
        return node->setInsetRight(value);
    }
    if (name == property::inset_bottom) {
        node->setInsetBottom(value);
        node->magicEnabled_ = true;
        return;
    }
    Super::onHandlePropTypeFloat(node_, parent, name_, value, reader);
}
} // namespace ui
} // namespace ee
