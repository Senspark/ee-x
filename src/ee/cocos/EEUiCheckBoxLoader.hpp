//
//  EEUiCheckBoxLoader.hpp
//  ee-library
//
//  Created by Zinge on 5/27/16.
//
//

#ifndef EE_LIBRARY_UI_CHECK_BOX_LOADER_HPP_
#define EE_LIBRARY_UI_CHECK_BOX_LOADER_HPP_

#include "EEMacro.hpp"
#include "EECocos2dxFwd.hpp"
#include "EEUiWidgetLoader.hpp"

#include <ui/UIWidget.h>

NS_EE_BEGIN
/// Cocosbuilder loader for @c cocos2d::ui::CheckBox.
class UiCheckBoxLoader : public UiWidgetLoader {
private:
    using Super = UiWidgetLoader;

public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(UiCheckBoxLoader, loader);

protected:
    UiCheckBoxLoader();
    
    virtual cocos2d::Node* createNode(cocos2d::Node* parent,
                                      cocosbuilder::CCBReader* reader) override;

    virtual cocos2d::SpriteFrame*
    parsePropTypeSpriteFrame(cocos2d::Node* node, cocos2d::Node* parent,
                             cocosbuilder::CCBReader* reader,
                             const char* propertyName) override;

    virtual void
    onHandlePropTypeCheck(cocos2d::Node* node, cocos2d::Node* parent,
                          const char* propertyName, bool check,
                          cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeFloat(cocos2d::Node* node, cocos2d::Node* parent,
                          const char* propertyName, float floatValue,
                          cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeSpriteFrame(cocos2d::Node* node, cocos2d::Node* parent,
                                const char* propertyName,
                                cocos2d::SpriteFrame* spriteFrame,
                                cocosbuilder::CCBReader* reader) override;

private:
    bool spriteFrameEnabled_;
    std::string spriteFrameName_;
    cocos2d::ui::Widget::TextureResType textureResType_;
};
NS_EE_END

#endif /* EE_LIBRARY_UI_CHECK_BOX_LOADER_HPP_ */
