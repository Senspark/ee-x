//
//  CCNodeV3Loader.cpp
//  ee-library
//
//  Created by Zinge on 5/10/16.
//
//

#include "EENodeV3Loader.hpp"

NS_EE_BEGIN
using Self = NodeV3Loader;

namespace property {
constexpr const char* local_z_order_overridden = "localZOrderOverridden";
constexpr const char* local_z_order = "localZOrder";
constexpr const char* color = "color";
constexpr const char* opacity = "opacity";
constexpr const char* cascade_color_enabled = "cascadeColorEnabled";
constexpr const char* cascade_opacity_enabled = "cascadeOpacityEnabled";
} // namespace property.

Self::NodeV3Loader()
    : localZOrderOverridden_(false) {}

cocos2d::Node* NodeV3Loader::createNode(cocos2d::Node* parent,
                                        cocosbuilder::CCBReader* reader) {
    return cocos2d::Node::create();
}

void NodeV3Loader::onHandlePropTypeCheck(cocos2d::Node* node,
                                         cocos2d::Node* parent,
                                         const char* propertyName, bool check,
                                         cocosbuilder::CCBReader* reader) {
    std::string propName(propertyName);
    if (propName == property::local_z_order_overridden) {
        localZOrderOverridden_ = check;
        return;
    }
    if (propName == property::cascade_color_enabled) {
        return node->setCascadeColorEnabled(check);
    }
    if (propName == property::cascade_opacity_enabled) {
        return node->setCascadeOpacityEnabled(check);
    }
    Super::onHandlePropTypeCheck(node, parent, propertyName, check, reader);
}

void NodeV3Loader::onHandlePropTypeColor3(cocos2d::Node* node,
                                          cocos2d::Node* parent,
                                          const char* propertyName,
                                          cocos2d::Color3B color3B,
                                          cocosbuilder::CCBReader* reader) {
    std::string propName(propertyName);
    if (propName == property::color) {
        return node->setColor(color3B);
    }
    Super::onHandlePropTypeColor3(node, parent, propertyName, color3B, reader);
}

void NodeV3Loader::onHandlePropTypeByte(cocos2d::Node* node,
                                        cocos2d::Node* parent,
                                        const char* propertyName,
                                        unsigned char byte,
                                        cocosbuilder::CCBReader* reader) {
    std::string propName(propertyName);
    if (propName == property::opacity) {
        return node->setOpacity(byte);
    }
    Super::onHandlePropTypeByte(node, parent, propertyName, byte, reader);
}

void NodeV3Loader::onHandlePropTypeInteger(cocos2d::Node* node,
                                           cocos2d::Node* parent,
                                           const char* propertyName,
                                           int integer,
                                           cocosbuilder::CCBReader* reader) {
    std::string propName(propertyName);
    if (propName == property::local_z_order) {
        if (localZOrderOverridden_) {
            node->setLocalZOrder(integer);
        }
        return;
    }
    Super::onHandlePropTypeInteger(node, parent, propertyName, integer, reader);
}
NS_EE_END
