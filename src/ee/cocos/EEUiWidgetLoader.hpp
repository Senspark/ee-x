//
//  EEUiWidgetLoader.hpp
//  ee-library
//
//  Created by Zinge on 5/5/16.
//
//

#ifndef EE_LIBRARY_UI_WIDGET_LOADER_HPP_
#define EE_LIBRARY_UI_WIDGET_LOADER_HPP_

#include "EEMacro.hpp"
#include "EECocos2dxFwd.hpp"
#include "EENodeV3Loader.hpp"

NS_EE_BEGIN
/// Cocosbuilder loader for @c cocos2d::ui::Widget.
///
/// Usage:
/// @code
/// your_node_loader_library->registerNodeLoader("CCWidget",
///                                               ee::UiWidgetLoader::loader());
/// @endcode
class UiWidgetLoader : public NodeV3Loader {
private:
    using Self = UiWidgetLoader;
    using Super = NodeV3Loader;

public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(Self, loader);

protected:
    virtual cocos2d::Node* createNode(cocos2d::Node* parent,
                                      cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeBlock(cocos2d::Node* node, cocos2d::Node* parent,
                          const char* propertyName,
                          cocosbuilder::BlockData* blockData,
                          cocosbuilder::CCBReader* ccbReader) override;

    virtual void
    onHandlePropTypeCheck(cocos2d::Node* node, cocos2d::Node* parent,
                          const char* propertyName, bool check,
                          cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeIntegerLabeled(cocos2d::Node* node, cocos2d::Node* parent,
                                   const char* propertyName, int integerLabeled,
                                   cocosbuilder::CCBReader* reader) override;
};
NS_EE_END

#endif /* EE_LIBRARY_UI_WIDGET_LOADER_HPP_ */
