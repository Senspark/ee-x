//
//  EEUiLayoutLoader.hpp
//  ee-library
//
//  Created by Zinge on 5/10/16.
//
//

#ifndef EE_LIBRARY_UI_LAYOUT_LOADER_HPP_
#define EE_LIBRARY_UI_LAYOUT_LOADER_HPP_

#include "EEMacro.hpp"
#include "EEUiWidgetLoader.hpp"

NS_EE_BEGIN
/// Cocosbuilder loader for @c cocos2d::ui::Layout.
///
/// Usage:
/// @code
/// your_node_loader_library->registerNodeLoader("CCLayout",
///                                               ee::UiLayoutLoader::loader());
/// @endcode
class UiLayoutLoader : public UiWidgetLoader {
private:
    using Super = UiWidgetLoader;

public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(UiLayoutLoader, loader);

protected:
    virtual cocos2d::Node* createNode(cocos2d::Node* parent,
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
};
NS_EE_END

#endif /* EE_LIBRARY_UI_LAYOUT_LOADER_HPP_ */
