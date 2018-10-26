//
//  EEClippingRectangleNodeLoader.cpp
//  ee-library
//
//  Created by Zinge on 2/13/17.
//
//

#include "EEClippingRectangleNodeLoader.hpp"

#include <2d/CCClippingRectangleNode.h>

NS_EE_BEGIN
namespace property {
constexpr auto content_size = "contentSize";
constexpr auto clipping_enabled = "clippingEnabled";
} // namespace property.

cocos2d::Node*
ClippingRectangleNodeLoader::createNode(cocos2d::Node* parent,
                                        cocosbuilder::CCBReader* reader) {
    return cocos2d::ClippingRectangleNode::create();
}

void ClippingRectangleNodeLoader::onHandlePropTypeCheck(
    cocos2d::Node* node, cocos2d::Node* parent, const char* propertyName,
    bool check, cocosbuilder::CCBReader* reader) {
    auto clipper = dynamic_cast<cocos2d::ClippingRectangleNode*>(node);
    std::string propName(propertyName);
    if (propName == property::clipping_enabled) {
        return clipper->setClippingEnabled(check);
    }
    Super::onHandlePropTypeCheck(node, parent, propertyName, check, reader);
}

void ClippingRectangleNodeLoader::onHandlePropTypeSize(
    cocos2d::Node* node, cocos2d::Node* parent, const char* propertyName,
    cocos2d::Size size, cocosbuilder::CCBReader* reader) {
    auto clipper = dynamic_cast<cocos2d::ClippingRectangleNode*>(node);
    std::string propName(propertyName);
    if (propName == property::content_size) {
        clipper->setClippingRegion(cocos2d::Rect(cocos2d::Point::ZERO, size));
    }
    Super::onHandlePropTypeSize(node, parent, propertyName, size, reader);
}
NS_EE_END
