//
//  EEClippingRectangleNodeLoader.hpp
//  ee-library
//
//  Created by Zinge on 2/13/17.
//
//

#ifndef EE_LIBRARY_CLIPPING_RECTANGLE_NODE_LOADER_HPP_
#define EE_LIBRARY_CLIPPING_RECTANGLE_NODE_LOADER_HPP_

#include "EENodeV3Loader.hpp"

NS_EE_BEGIN
class ClippingRectangleNodeLoader : public NodeV3Loader {
private:
    using Super = NodeV3Loader;

public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ClippingRectangleNodeLoader,
                                             loader);

protected:
    virtual cocos2d::Node* createNode(cocos2d::Node* parent,
                                      cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeCheck(cocos2d::Node* node, cocos2d::Node* parent,
                          const char* propertyName, bool check,
                          cocosbuilder::CCBReader* reader) override;

    virtual void onHandlePropTypeSize(cocos2d::Node* node,
                                      cocos2d::Node* parent,
                                      const char* propertyName,
                                      cocos2d::Size size,
                                      cocosbuilder::CCBReader* reader) override;
};
NS_EE_END

#endif /* EE_LIBRARY_CLIPPING_RECTANGLE_NODE_LOADER_HPP_ */
