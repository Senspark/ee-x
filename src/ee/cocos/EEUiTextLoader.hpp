//
//  EEUiTextLoader.hpp
//  ee-library
//
//  Created by enrevol on 3/25/16.
//
//

#ifndef EE_LIBRARY_UI_TEXT_LOADER_HPP_
#define EE_LIBRARY_UI_TEXT_LOADER_HPP_

#include "EEMacro.hpp"
#include "EEUiWidgetLoader.hpp"

NS_EE_BEGIN
/// Cocosbuilder loader for @c cocos2d::ui::Text.
///
/// Usage:
/// @code
/// your_node_loader_library->registerNodeLoader("CCText",
///                                               ee::UiTextLoader::loader());
/// @endcode
class UiTextLoader : public UiWidgetLoader {
private:
    using Super = UiWidgetLoader;

public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(UiTextLoader, loader);

protected:
    UiTextLoader();
    
    virtual cocos2d::Node* createNode(cocos2d::Node* parent,
                                      cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypePoint(cocos2d::Node* node, cocos2d::Node* parent,
                          const char* propertyName, cocos2d::Vec2 point,
                          cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeCheck(cocos2d::Node* node, cocos2d::Node* parent,
                          const char* propertyName, bool check,
                          cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeColor3(cocos2d::Node* node, cocos2d::Node* parent,
                           const char* propertyName, cocos2d::Color3B color3B,
                           cocosbuilder::CCBReader* reader) override;

    virtual void onHandlePropTypeByte(cocos2d::Node* node,
                                      cocos2d::Node* parent,
                                      const char* propertyName,
                                      unsigned char byte,
                                      cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeFontTTF(cocos2d::Node* node, cocos2d::Node* parent,
                            const char* propertyName, const char* fontTTF,
                            cocosbuilder::CCBReader* reader) override;

    virtual void onHandlePropTypeText(cocos2d::Node* node,
                                      cocos2d::Node* parent,
                                      const char* propertyName,
                                      const char* text,
                                      cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeFloatScale(cocos2d::Node* node, cocos2d::Node* parent,
                               const char* propertyName, float floatScale,
                               cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeInteger(cocos2d::Node* node, cocos2d::Node* parent,
                            const char* propertyName, int integer,
                            cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeIntegerLabeled(cocos2d::Node* node, cocos2d::Node* parent,
                                   const char* propertyName, int integerLabeled,
                                   cocosbuilder::CCBReader* reader) override;

    virtual void onHandlePropTypeSize(cocos2d::Node* node,
                                      cocos2d::Node* parent,
                                      const char* propertyName,
                                      cocos2d::Size size,
                                      cocosbuilder::CCBReader* reader) override;

private:
    bool shadowEnabled_;
    GLubyte shadowOpacity_;
    cocos2d::Color3B shadowColor_;
    cocos2d::Size shadowOffset_;
    int shadowBlurRadius_;

    bool outlineEnabled_;
    GLubyte outlineOpacity_;
    cocos2d::Color3B outlineColor_;
    int outlineSize_;
};
NS_EE_END

#endif /* EE_LIBRARY_UI_TEXT_LOADER_HPP_ */
