//
//  CCNodeV3Loader.hpp
//  ee-library
//
//  Created by Zinge on 5/10/16.
//
//

#ifndef EE_LIBRARY_NODE_V3_LOADER_HPP_
#define EE_LIBRARY_NODE_V3_LOADER_HPP_

#include "EEMacro.hpp"

#include <cocosbuilder/CCNodeLoader.h>

NS_EE_BEGIN
/// Cocosbuilder loader for @c cocos2d::Node with color and opacity.
///
/// Usage:
/// @code
/// your_node_loader_library->registerNodeLoader("CCNodeV3",
///                                               ee::NodeV3::loader());
/// @endcode
class NodeV3Loader : public cocosbuilder::NodeLoader {
private:
    using Super = cocosbuilder::NodeLoader;

public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(NodeV3Loader, loader);

protected:
    NodeV3Loader();
    
    virtual cocos2d::Node* createNode(cocos2d::Node* parent,
                                      cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeColor3(cocos2d::Node* node, cocos2d::Node* parent,
                           const char* propertyName, cocos2d::Color3B color3B,
                           cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeCheck(cocos2d::Node* node, cocos2d::Node* parent,
                          const char* propertyName, bool check,
                          cocosbuilder::CCBReader* reader) override;

    virtual void onHandlePropTypeByte(cocos2d::Node* node,
                                      cocos2d::Node* parent,
                                      const char* propertyName,
                                      unsigned char byte,
                                      cocosbuilder::CCBReader* reader) override;

    virtual void
    onHandlePropTypeInteger(cocos2d::Node* node, cocos2d::Node* parent,
                            const char* propertyName, int integer,
                            cocosbuilder::CCBReader* reader) override;

private:
    bool localZOrderOverridden_;
};
NS_EE_END

#endif /* EE_LIBRARY_NODE_V3_LOADER_HPP_ */
