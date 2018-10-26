//
//  EEWidgetLoader.hpp
//  ee-library
//
//  Created by eps on 6/4/18.
//

#ifndef EE_LIBRARY_WIDGET_LOADER_HPP
#define EE_LIBRARY_WIDGET_LOADER_HPP

#include "EEForward.hpp"
#include "EEUiWidgetLoader.hpp"

namespace ee {
namespace ui {
class WidgetLoader : public UiWidgetLoader {
private:
    using Self = WidgetLoader;
    using Super = UiWidgetLoader;

public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(Self, loader);

protected:
    virtual cocos2d::Node* createNode(cocos2d::Node* parent,
                                      cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeFloat(cocos2d::Node* node, cocos2d::Node* parent,
                          const char* name, float value,
                          cocosbuilder::CCBReader* reader) override;
};
} // namespace ui
} // namespace ee

#endif /* EE_LIBRARY_WIDGET_LOADER_HPP */
